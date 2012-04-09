%include 'definitions.h.asm'

%macro puts 1
    call _puts
    db %1, 0
%endmacro

[ORG 0x00007C00]

[BITS 16]

boot_loader:
    ;Parameter from BIOS: dl = boot drive

    ; sane default state
    xor ax,ax
    mov ds,ax
    mov es,ax
    mov ss,ax

    push ax
    popf

    mov sp,0x7C00

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

enter_unreal_mode:
    lgdt [gdt.pointer]

    push ds

    mov eax, cr0
    or al, 1 ; protected mode enabled
    mov cr0, eax

    mov bx, gdt.data
    mov ds, bx

    and al, ~1
    mov cr0, eax
    
    pop ds

load_image:
    pop dx

    puts "Prepare for chaOS "

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

    puts "."

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
    puts {13,10,"Disk failure while loading chaOS!",13,10}
    int 0x18

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

enter_second_stage:
.copy:
    mov esi, KERNEL_BASE + ELF64_HEADER_SIZE
    mov di, SECOND_STAGE_BASE + ELF64_HEADER_SIZE
    mov cx, (SECOND_STAGE_MAX_SIZE - ELF64_HEADER_SIZE) / 4
.loop:
    mov eax, [esi]
    add esi, 4
    stosd
    loop .loop
.enter:
    jmp SECOND_STAGE_BASE + ELF64_HEADER_SIZE

%include 'puts.inc.asm'

    ;Global Descriptor Table
gdt:
.null equ $ - gdt
    dq 0x0000000000000000
.code equ $ - gdt
    dq 0x0020980000000000
.data equ $ - gdt
    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
.pointer:
    dw $-gdt-1        ;16-bit Size (Limit)
    dq gdt            ;64-bit Base Address

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

; fail if bootloader becomes to big
    times 446-($-$$) db 0x90        ;Fill boot sector
