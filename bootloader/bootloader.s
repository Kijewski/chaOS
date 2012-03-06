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
[BITS 16]

boot_loader:
    ;Parameter from BIOS: dl = boot drive

    ;Set default state
    cli

    xor eax,eax
    mov ds,ax
    mov es,ax
    mov ss,ax

    mov esp,0x7C00

    push eax
    popfd

    jmp 0:.clear_cs
.clear_cs:

enabled_a20:
    ;Enable A20 via port 92h
    in al,92h
    or al,02h
    out 92h,al

    push dx ; preserve current boot device

read_e820:
    xor ebx, ebx
    mov di, 0x4504
.loop:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 0x534D4150

    int 0x15
    jc fail
    cmp eax, 'PAMS'
    jne fail

    mov word [di-4], cx
    add di, cx
    add di, 4

    test ebx, ebx
    jnz .loop
.end:
    mov word [di-2], 0

enter_unreal_mode:
    lgdt [gdt.pointer]

    push ds

    mov eax, cr0
    or al, 1
    mov cr0, eax

    mov bx, gdt.unreal
    mov ds, bx

    and al, 0xFE
    mov cr0, eax
    
    pop ds

load_image:
    pop dx
    push dx

    xor ax, ax
    int 0x13
    jc fail

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc fail

    cmp word [0x0500 + 510], 0xaa55
    jne fail

    mov edi, KERNEL_BASE

    mov ebx, dword [0x0500 + 446+16*(PARTITION-1) + 8]
    mov [dap.start], ebx
    push ebx
    mov ecx, dword [0x0500 + 446+16*(PARTITION-1) + 12]
    push ecx

    cmp ecx, 1024*1024/512
    jbe .not_too_big
    mov ecx, 1024*1024/512
.not_too_big:

.read_sector:
    mov ebx, MAX_SECTORS_AT_ONCE
    cmp ecx, ebx
    jae .not_too_much
    mov bx, cx
.not_too_much:
    mov [dap.count], bx

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc fail
    
    mov bx, [dap.count]
    movzx ebx, bx
    add [dap.start], ebx
    sub ecx, ebx

    mov si, 0x0500
    xor eax, eax
    shl ebx, 7 ; ld (512/4)
.copy:
    sub ebx, 1
    js .cont

    mov eax, [si]
    mov [ds:edi], eax
    add si, 4
    add edi, 4
    jmp .copy

.cont:
    jecxz build_temp_pagetable
    jmp .read_sector

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

    ;PD:
    ;dq 0x000000000000018f = 00000000 00000000 00000000 00000000 00000000 00000000 00000001 10001111
    ;times 511 dq 0x0000000000000000

    ;This defines one 2MB page at the start of memory, so we can access the first 2MBs as if paging was disabled

    ; build the necessary tables
    xor bx,bx
    mov es,bx
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

    ;PD:
    mov ax,0x018f
    stosw

    xor ax,ax
    mov cx,0x07ff
    rep stosw

.enter_long_mode:
    ;Enter long mode

    mov eax,10100000b       ;Set PAE and PGE
    mov cr4,eax

    mov edx, 0x0000a000       ;Point CR3 at PML4
    mov cr3,edx

    mov ecx,0xC0000080        ;Specify EFER MSR

    rdmsr           ;Enable Long Mode
    or eax,0x00000100
    wrmsr

    mov ebx,cr0         ;Activate long mode
    or ebx,0x80000001       ;by enabling paging and protection simultaneously
    mov cr0,ebx         ;skipping protected mode entirely

    sub word [gdt.pointer], 8 ; delete unreal descriptor
    lgdt [gdt.pointer]        ;load 80-bit gdt.pointer below

    jmp gdt.code:startLongMode      ;Load CS with 64 bit segment and flush the instruction cache

    ;Global Descriptor Table
gdt:
    dq 0x0000000000000000       ;Null Descriptor
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

.signature:
    times 446-($-$$) db 0				;Fill boot sector
