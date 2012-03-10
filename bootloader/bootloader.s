; Crude composition of
;   * http://wiki.osdev.org/index.php?title=Talk:Entering_Long_Mode_Directly&oldid=12173
;     by http://wiki.osdev.org/User:Oculusfervoris
; and
;   * http://wiki.osdev.org/index.php?title=Unreal_Mode&oldid=10705
;     by http://wiki.osdev.org/User:Jhawthorn

[ORG 0x00007C00]
KERNEL_BASE equ 1024*1024
ELF64_ENTRY_POINT equ 3*8

PARTITION equ 1

MAX_SECTORS_AT_ONCE equ 32
E820_MAP_BASE equ 0x0500 + 0x200*MAX_SECTORS_AT_ONCE ; 0x4500

[BITS 16]

boot_loader:
    ;Parameter from BIOS: dl = boot drive

    ; sane default state
    cli

    xor ax,ax
    mov ds,ax
    mov es,ax
    mov ss,ax

    push ax
    popf

    mov esp,0x7C00

    push dx ; preserve current boot device

    ; BIOSes are reportedly allowed to start at 7C00:0000
    ; instead of the assumed 0000:7C000
    jmp 0:.clear_cs
.clear_cs:

enabled_a20:
    ;Enable A20 via port 92h
    in al,92h
    or al,02h
    out 92h,al

read_e820:
    xor ebx, ebx
    mov di, E820_MAP_BASE+2
.loop:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 'PAMS'

    int 0x15
    jc fail
    cmp eax, 'PAMS'
    jne fail

    mov word [di-2], cx
    add di, cx
    add di, 2

    test ebx, ebx
    jnz .loop
.end:
    mov word [di-2], bx ; bx is zero

enter_unreal_mode:
    lgdt [gdt.pointer]

    push ds

    mov eax, cr0
    or al, 1 ; protected mode enabled
    mov cr0, eax

    mov bx, gdt.unreal
    mov ds, bx

    and al, ~1
    mov cr0, eax
    
    pop ds

load_image:
    pop dx

    ; copy kernel image
    mov edi, KERNEL_BASE

    mov ebx, dword [0x7C00 + 446+16*(PARTITION-1) + 8] ; start LBA sector
    mov [dap.start], ebx
    mov ecx, dword [0x7C00 + 446+16*(PARTITION-1) + 12] ; sectors count

.read_sector:
    mov ebx, MAX_SECTORS_AT_ONCE
    cmp ebx, ecx
    cmova ebx, ecx

    mov [dap.count], bx

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc .read_sector_again

    mov bx, [dap.count]
    movzx ebx, bx
    add [dap.start], ebx
    sub ecx, ebx

    mov si, 0x0500
    shl ebx, 7 ; ld (512/4)
.copy:
    sub ebx, 1
    js .cont

    lodsd
    mov [ds:edi], eax ; movsd/stosd seem to ignore the "unreal mode"
    add edi, 4

    jmp .copy

.cont:
    jecxz build_temp_pagetable
    jmp .read_sector

.read_sector_again:
    ; reset disk controller
    xor ax, ax
    int 0x13
    jnc .read_sector

fail:
    int 0x18

dap:
    db 0x10 ; size
    db 0x00 ; reserved
.count:
    dw 0x0001
.offs:
    dw 0x0500
    dw 0x0000
.start:
    dq 0x0000000000000000

build_temp_pagetable:
    ;Build page tables
    ;The page tables will look like this:
    ;PML4:
    ;dq 0x000000000000b00f = 00000000 00000000 00000000 00000000 00000000 00000000 10010000 00001111
    ;times 511 dq 0x0000000000000000

    ;PDP:
    ;dq 0x000000000000c00f = 00000000 00000000 00000000 00000000 00000000 00000000 10100000 00001111
    ;times 511 dq 0x0000000000000000

    ;                              56       48       40       32       24       16        8        0
    ;PD:                                                                     rrrrr rrrr---G S0ADWURP
    ;dq 0x000000000000018f = 00000000 00000000 00000000 00000000 00000000 00000000 00000001 10001111
    ;dq 0x000000000000018f = 00000000 00000000 00000000 00000000 00000000 00100000 00000001 10001111
    ;dq 0x000000000000018f = 00000000 00000000 00000000 00000000 00000000 01000000 00000001 10001111
    ;dq 0x000000000000018f = 00000000 00000000 00000000 00000000 00000000 01100000 00000001 10001111
    ;...

    ;This defines one 512*2MB pages at the start of memory, so we can access the first 1GB as if paging was disabled

    ; build the necessary tables
    mov di,0xa000
    
    ;PML4:
    mov ax,0xb00f
    stosw

    xor ax,ax
    mov cx,0x07ff
    rep stosw

    ;PDP:
    mov ax,0xc00f
    stosw

    xor ax,ax
    mov cx,0x07ff
    rep stosw

    mov ebx, 0b00000000000000000000000110001111
    mov cx, 512
.build_pd:
    mov eax, ebx
    stosd
    add ebx, 1<<21
    xor ax, ax
    stosw
    stosw
    loop .build_pd

.enter_long_mode:
    ; Set PAE and PGE
    mov eax,10100000b
    mov cr4,eax

    ; Point CR3 at PML4
    mov edx, 0x0000a000
    mov cr3,edx

    ; Specify EFER MSR
    mov ecx,0xC0000080

    ; Enable Long Mode
    rdmsr
    or ah, 1
    wrmsr

    mov ebx,cr0       ; Activate long mode
    or ebx,0x80000001 ; by enabling paging and protection simultaneously
    mov cr0,ebx       ; skipping protected mode entirely

    sub word [gdt.pointer], 8 ; delete unreal descriptor
    lgdt [gdt.pointer]        ; load 80-bit gdt.pointer below

     ; Load CS with 64 bit segment and flush the instruction cache
    jmp gdt.code:startLongMode

    ;Global Descriptor Table
gdt:
.null equ $ - gdt
    dq 0x0000000000000000
.code equ $ - gdt
    dq 0x0020980000000000
.data equ $ - gdt
    dq 0x0000900000000000
.unreal equ $ - gdt
    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
.pointer:
    dw $-gdt-1        ;16-bit Size (Limit)
    dq gdt            ;64-bit Base Address
                      ;Changed from "dd gdt"
                      ;Ref: Intel System Programming Manual V1 - 2.1.1.1

[BITS 64]
startLongMode:
    jmp [KERNEL_BASE + ELF64_ENTRY_POINT]


; fail if bootloader becomes to big
    times 446-($-$$) db 0x90				;Fill boot sector
