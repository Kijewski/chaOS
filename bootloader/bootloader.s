; Crude composition of
;   * http://wiki.osdev.org/index.php?title=Talk:Entering_Long_Mode_Directly&oldid=12173
;     by http://wiki.osdev.org/User:Oculusfervoris
; and
;   * http://wiki.osdev.org/index.php?title=Unreal_Mode&oldid=10705
;     by http://wiki.osdev.org/User:Jhawthorn

[ORG 0x00007C00]
kernel_base equ 1024*1024
elf64_entry_point equ 3*8

partition equ 1
[BITS 16]

%macro halt 0
    jmp _halt
%endmacro

boot_loader:
    ;Parameter from BIOS: dl = boot drive

    ;Set default state
    cli

    xor eax,eax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    mov esp,0x7C00
    mov ebp,esp

    push eax
    popfd

    jmp 0:.clear_cs
.clear_cs:

.enabled_a20:
    ;Enable A20 via port 92h
    in al,92h
    or al,02h
    out 92h,al

.enter_unreal_mode:
    push ds

    lgdt [gdt.pointer]

    mov eax, cr0
    or al, 1
    mov cr0, eax

    mov bx, gdt.unreal
    mov ds, bx

    and al, 0xFE
    mov cr0, eax
    
    pop ds

    ; dx was left unchanged, still contains device number
    push edx

load_image:
    xor ax, ax
    int 0x13
    jc .load_image_failed

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc .load_image_failed

    cmp word [0x0500 + 510], 0xaa55
    jne .load_image_failed

    mov edi, kernel_base

    mov ebx, dword [0x0500 + 446+16*(partition-1) + 8]
    mov [dap.start], ebx
    push ebx
    mov ecx, dword [0x0500 + 446+16*(partition-1) + 12]
    push ecx

    cmp ecx, 1024*1024/512
    jbe .not_too_big
    mov ecx, 1024*1024/512
.not_too_big:

.read_sector:
    mov ebx, 32
    cmp ecx, ebx
    jae .not_too_much
    mov bx, cx
.not_too_much:
    mov [dap.count], bx

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc .load_image_failed
    
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

.load_image_failed:
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

    pop esi
    pop edi
    pop edx

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

_halt:
    hlt
    jmp _halt

[BITS 64]
startLongMode:
    ; rdi, rsi and rdx are set
    mov rax, [kernel_base + elf64_entry_point]
    call rax

    halt

.signature:
    times 446-($-$$) db 0				;Fill boot sector
    
