#include "nop.h"

#define ROUND             \
        "xor %rax, %rbx;" \
        "xor %rbx, %rcx;" \
        "xor %rcx, %rdx;" \
        "xor %rdx, %r8;"  \
                          \
        "xor %r8,  %r9;"  \
        "xor %r9,  %r10;" \
        "xor %r10, %r11;" \
        "xor %r11, %r12;" \
                          \
        "xor %r12, %r13;" \
        "xor %r13, %r14;" \
        "xor %r14, %r15;" \
        "xor %r15, %rdi;" \
                          \
        "xor %rdi, %rsi;" \
        "xor %rsi, %rax;"

#define ROL               \
        ROUND             \
        ROUND             \
        ROUND             \
        ROUND             \
                          \
        ROUND             \
        ROUND             \
        ROUND             \
        ROUND             \
        ROUND             \
                          \
        ROUND             \
        ROUND             \
        ROUND             \
        ROUND             \
                          \
        ROUND             \
        ROUND             \
                          \
        "xor %rsi, %rax;"

// 844 cache stalling operations!
void 
expensive_nop (void)
{
  asm volatile (ROL ROL ROL ROL);
}
