%include 'definitions.h.asm'

[ORG SECOND_STAGE_BASE + ELF64_HEADER_SIZE]

[BITS 16]

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
    jz fail

.detect_cpuid_gt_80000000:
    mov eax, 0x80000000    ; Set the A-register to 0x80000000.
    mov esi, eax
    inc si
    cpuid                  ; CPU identification.
    cmp eax, esi    ; Compare the A-register with 0x80000001.
    jb fail         ; It is less, there is no long mode.

.detect_long_mode:
    mov eax, esi    ; Set the A-register to 0x80000001.
    cpuid                  ; CPU identification.
    bt edx, 29      ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jnc fail         ; They aren't, there is no long mode.

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

set_console:
    ; set 80x25 text mode so we're in a known state, and to set 8x16 font
    mov ax,0083h
    int 10h
  
    ; set 80x50 text mode and 8x8 font
    mov ax,1112h
    xor bl,bl
    int 10h

enter_long_mode:
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

    mov ebx,cr0       ; Activate long mode
    or ebx,0x80000001 ; by enabling paging and protection simultaneously
    mov cr0,ebx       ; skipping protected mode entirely

     ; Load CS with 64 bit segment and flush the instruction cache
    jmp 8:startLongMode

fail:
    int 0x18

[BITS 64]
startLongMode:
    jmp [KERNEL_BASE + ELF64_ENTRY_POINT]

    times 0x1000 - ($-$$) - ELF64_HEADER_SIZE db 0x90
