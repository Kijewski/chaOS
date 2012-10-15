#ifndef INTERRUPTS__
#define INTERRUPTS__

#include <common/attributes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

struct idt_entry
{
// 0x00
  uint16_t offset_low;
// 0x02
  uint16_t selector;
// 0x04
  uint8_t  zero1;
// 0x05
  uint8_t  type;
// 0x06
  uint16_t offset_mid;
// 0x08
  uint32_t offset_high;
// 0x0C
  uint32_t zero2;
// 0x10
} PACKED;

enum
{
  INT_DIV_ZERO                      =  0,
  INT_DEBUG                         =  1,
  INT_NMI                           =  2,
  INT_BREAKPOINT                    =  3,
  INT_OVERFLOW                      =  4,
  INT_BOUNDS                        =  5,
  INT_INVALID_OPCODE                =  6,
  INT_COPROCESSOR_NOT_AVAILABLE     =  7,
  INT_DOUBLE_FAULT                  =  8,
  INT_COPROCESSOR_SEGMENT_OVERRUN   =  9,
  INT_INVALID_TASK_STATE_SEGEMENT   = 10,
  INT_SEGMENT_NOT_PRESENT           = 11,
  INT_STACK_FAULT                   = 12,
  INT_GENERAL_PROTECTION_FAULT      = 13,
  INT_PAGE_FAULT                    = 14,
                                   // 15,
  INT_MATH_FAULT                    = 16,
  INT_ALIGNMENT_CHECK               = 17,
  INT_MACHIENE_CHECK                = 18,
  INT_SIMD_FLOATION_POINT_EXCEPTION = 19,

  INT_SPECIAL_DEBUG = 0xCC,
  INT_SYSCALL       = 0xA0,
};

extern const struct idt_entry idt_entries[256];

struct interrupt_frame
{
  uint64_t rax, rbx, rcx, rdx;
  uint64_t rdi, rsi, rsp, rbp;
  uint64_t r8,  r9,  r10, r11;
  uint64_t r12, r13, r14, r15;
  uint64_t flags;
} PACKED;

typedef void intr_handler_fun (int num, struct interrupt_frame *f);

bool interrupts_init (void);
void interrupts_finit (void);

void interrupts_set_handler (int num, intr_handler_fun *fun);

struct interrupt_frame *intr_handler (int num, struct interrupt_frame *f);

#endif
