[BITS 64]

extern intr_handler

intr_handler_x:
  mov [0x10+rax], rcx
  mov [0x18+rax], rdx
  mov [0x20+rax], rdi
  mov [0x28+rax], rsi
  mov [0x30+rax], rsp
  mov [0x38+rax], rbp
  mov [0x40+rax], r8
  mov [0x48+rax], r9
  mov [0x50+rax], r10
  mov [0x58+rax], r11
  mov [0x60+rax], r12
  mov [0x68+rax], r13
  mov [0x70+rax], r14
  mov [0x78+rax], r15

  pushf
  pop qword [0x80+rax]

  movzx edi, bl
  mov rsi, rax

  mov rax, qword intr_handler
  call rax

  push qword [0x80+rax]
  popf

  mov rbx, [0x08+rax]
  mov rcx, [0x10+rax]
  mov rdx, [0x18+rax]
  mov rdi, [0x20+rax]
  mov rsi, [0x28+rax]
  mov rsp, [0x30+rax]
  mov rbp, [0x38+rax]
  mov r8 , [0x40+rax]
  mov r9 , [0x48+rax]
  mov r10, [0x50+rax]
  mov r11, [0x58+rax]
  mov r12, [0x60+rax]
  mov r13, [0x68+rax]
  mov r14, [0x70+rax]
  mov r15, [0x78+rax]
  mov rax, [rax]

  iretq

%macro INTR_HANDLER 1
  SECTION .text
    align 16
    global intr_handler_%1
    intr_handler_%1:
      mov [abs qword .data], rax
      mov rax, qword .data
      mov [0x08+rax], rbx
      mov bl, %1
      jmp intr_handler_x
  SECTION .bss
    .data:
      resb 17 * 8
%endmacro

%assign i 0 
%rep    256
  INTR_HANDLER i
%assign i i+1 
%endrep
