%include 'definitions.h.asm'

[ORG SECOND_STAGE_BASE + ELF64_HEADER_SIZE]

%macro puts 1
    pusha
    mov si, %1
    call _puts
    popa
%endmacro

[BITS 16]

second_stage:
    puts strings.image_loaded

read_e820:
    xor ebx, ebx
    mov di, E820_MAP_BASE+2
.loop:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 'PAMS'

    int 0x15
    jc .fail
    cmp eax, 'PAMS'
    jne .fail

    mov word [di-2], cx
    lea di, [edi+ecx+2]

    test ebx, ebx
    jnz .loop
.end:
    mov word [di-2], bx ; bx is zero
    jmp build_temp_pagetable
.fail:
    puts strings.bios_no_e820
    jmp fail

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
    xor eax, eax
    stosd
    loop .build_pd

enter_long_mode:
    puts strings.entering_long_mode

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
    or ah, ((1<<8) | (1<<11)) >> 8 ; LMA + NXE
    wrmsr

    call set_text_mode ; don't call puts afterwards!

    mov ebx,cr0       ; Activate long mode
    or ebx,0x80000001 ; by enabling paging and protection simultaneously
    mov cr0,ebx       ; skipping protected mode entirely

     ; Load CS with 64 bit segment and flush the instruction cache
    jmp 8:startLongMode

fail:
    puts strings.chaos_prevented
    int 0x18

_puts:
    mov bx, 0x0007
    mov cx, 1

.loop:
    lodsb

    test al, al
    jz .end

    cmp al, 27
    je .escape

    cmp al, 32
    jb .non_printable

.printable:
    mov ah, 0x09
    int 10h

.non_printable:
    mov ah, 0x0E
    int 10h
    jmp .loop

.escape:
    lodsb
    mov bl, al
    jmp .loop

.end:
    ret

%include 'textmode.inc.asm'

[BITS 64]
startLongMode:
    jmp [KERNEL_BASE + ELF64_ENTRY_POINT]

strings:
.image_loaded:
    db 13, 10, 27, 8Ch, " chaOS ", 27, 7, " image loaded", 13, 10, 0
.bios_no_e820:
    db 13, 10, 27, 8Ch, "Your BIOS does not support e820 or reported an error.", 13, 10, 0
.entering_long_mode:
    db "Entering long mode ...", 13, 10, 0
.chaos_prevented:
    db 13, 10, 27, 8Ch, " chaOS ", 27, 7, " prevented :-(", 13, 10, 13, 10, 0

times 0x1000 - ($-$$) - ELF64_HEADER_SIZE db 0x90
