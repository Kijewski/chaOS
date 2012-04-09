%include 'definitions.h.asm'

[ORG SECOND_STAGE_BASE + ELF64_HEADER_SIZE]

%macro putsln 1
    call _puts
    db %1, 13, 10, 0
%endmacro

[BITS 16]
    putsln {13,10,"chaOS image loaded"}

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
    jnz .detect_cpuid_gt_80000000

    putsln "Your PC does not support the cpuid instruction."
    jmp fail

; http://www.sandpile.org/x86/cpuid.htm#level_8000_0000h
.detect_cpuid_gt_80000000:
    mov eax, 0x80000000          ; Set the A-register to 0x80000000.
    cpuid                        ; CPU identification.
    cmp eax, 0x80000001          ; Compare the A-register with 0x80000001.
    jae .detect_features         ; It is less, there is no long mode.

    putsln "Your CPU does not support extended level 8000 0001h."
    jmp fail

; http://www.sandpile.org/x86/cpuid.htm#level_8000_0001h
.detect_features:
    mov eax, esi           ; Set the A-register to 0x80000001.
    cpuid                  ; CPU identification.
.lm:
    bt edx, 29             ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jnc .nx                ; They aren't, there is no long mode.
    putsln "Your CPU does not support long mode."
    jmp fail
.nx:
    bt edx, 20
    jnc read_e820
    putsln "Your CPU does not support the NX / XD bit."
    jmp fail

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
    add di, cx
    add di, 2

    test ebx, ebx
    jnz .loop
.end:
    mov word [di-2], bx ; bx is zero
    jmp enter_long_mode
.fail:
    putsln "Yout BIOS does not support e820 or reported an error."
    jmp fail

enter_long_mode:
    putsln "Entering long mode ..." ; invisible

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
    int 0x18

%include 'puts.inc.asm'
%include 'textmode.inc.asm'

[BITS 64]
startLongMode:
    jmp [KERNEL_BASE + ELF64_ENTRY_POINT]

    times 0x1000 - ($-$$) - ELF64_HEADER_SIZE db 0x90
