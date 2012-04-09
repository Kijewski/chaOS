_puts:
    pusha
    xchg si, [esp+16]

    mov ah, 0x0E
    mov bx, 0x0007
.loop:
    lodsb
    test al, al
    jz .end
    int 10h
    jmp .loop

.end:
    xchg si, [esp+16]
    popa
    ret
