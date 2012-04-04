#include "nop.h"

#define ROUND             \
        "xor %rax, %rbx;" \
        "xor %rbx, %rcx;" \
        "xor %rcx, %rdx;" \
        "xor %rdx, %r8;"  \

#define ROL               \
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
}
