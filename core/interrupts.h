#ifndef INTERRUPTS__
#define INTERRUPTS__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <attributes.h>

struct interrupt_frame
{
  uint64_t rax, rbx, rcx, rdx;
  uint64_t rdi, rsi, rsp, rbp;
  uint64_t r8,  r9,  r10, r11;
  uint64_t r12, r13, r14, r15;
} PACKED;

typedef void intr_handler_fun (int num, struct interrupt_frame *f);

bool interrupts_init (void);
void interrupts_finit (void);

void interrupts_set_handler (int num, intr_handler_fun *fun);

#endif
