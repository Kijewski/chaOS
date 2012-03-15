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
        "xor %r11, %rax;" \

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
                          \
        "xor %r11, %rax;"

void 
expensive_nop (void)
{
  asm volatile (ROL);
  asm volatile (ROL);
  asm volatile (ROL);
  asm volatile (ROL);

  asm volatile (ROL);
  asm volatile (ROL);
  asm volatile (ROL);
  asm volatile (ROL);
}
