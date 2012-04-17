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

welcome:
    puts "Prepare for chaOS "

detect_amd64:
.detect_cpuid:
    pushfd
    mov ecx, [esp]
    btc long [esp], 21
    popfd

    pushfd
    pop eax

    push ecx
    popfd

    xor eax, ecx
    jz .fail

; http://www.sandpile.org/x86/cpuid.htm#level_8000_0000h
.detect_cpuid_gt_80000000:
    mov eax, 0x80000000          ; Set the A-register to 0x80000000.
    mov esi, eax
    inc esi
    cpuid                        ; CPU identification.
    cmp eax, esi                 ; Compare the A-register with 0x80000001.
    jb .fail                     ; It is less, there is no long mode.

; http://www.sandpile.org/x86/cpuid.htm#level_8000_0001h
.detect_features:
    mov eax, esi
    cpuid
    mov eax, (1<<20) | (1<<29) ; NXE | LM
    and edx, eax
    cmp eax, edx
    je enabled_a20

.fail:
    puts {13,10,"Your CPU does not support long mode.",13,10}
    int 0x18

enabled_a20:
    ;Enable A20 via port 92h
    in al,92h
    or al,02h
    and al,0feh
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

    ; copy kernel image
    mov edi, KERNEL_BASE

    mov ebx, dword [0x7C00 + 446+16*(PARTITION-1) + 8] ; start LBA sector
    mov [dap.start], ebx
    mov ecx, dword [0x7C00 + 446+16*(PARTITION-1) + 12] ; sectors count

.read_sector:
    mov ebx, MAX_SECTORS_AT_ONCE
    cmp ebx, ecx
    jb .read_sector_not_too_much
    mov ebx, ecx
.read_sector_not_too_much

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
    jecxz enter_second_stage
    jmp .read_sector

.read_sector_again:
    ; reset disk controller
    xor ax, ax
    int 0x13
    jnc .read_sector

.fail:
    puts {13,10,"Disk failure while loading chaOS!",13,10}
    int 0x18

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
