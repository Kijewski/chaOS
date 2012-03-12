// TODO: translate whole file to (n)asm

#include "interrupts.h"
#include "kmain.h"
#include <assert.h>
#include <string.h>
#include <attributes.h>

enum {
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
};

enum
{
  IDT_TASK_GATE      = 0b1101,
  IDT_INTERRUPT_GATE = 0b1110,
  IDT_TRAP_GATE      = 0b1111,

  IDT_DPL_0 = 0 << 5,
  IDT_DPL_1 = 1 << 5,
  IDT_DPL_2 = 2 << 5,
  IDT_DPL_3 = 3 << 5,

  IDT_ENTRY_PRESENT = 1 << 7,
};

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
GLOBAL_CASSERT (sizeof (struct idt_entry) == 0x10)

struct idtr
{
  uint16_t limit;
  uint64_t offset;
} PACKED;

static intr_handler_fun *funs[32];

#include "kmain.h"

static void
intr_default_handler (int num, struct interrupt_frame *f UNUSED)
{
  videoram_puts ("\n Got unhandled interrupt 0x", COLOR_ERROR);
  videoram_put_hex (num, COLOR_ERROR);
  videoram_puts ("! \n", COLOR_ERROR);
  khalt ();
}

#define INTR_HANDLER(NUM)                                                     \
static void                                                                   \
CASSERT_CONCAT_ (intr_handler_, NUM) (void)                                   \
{                                                                             \
  static struct interrupt_frame f;                                            \
  static uint8_t stack[0x1000];                                               \
                                                                              \
  asm volatile ("mov %%rax, %0;"                                              \
                "lea %0, %%rax;"                                              \
                "mov %%rbx, 0x08(%%rax);"                                     \
                "mov %%rcx, 0x10(%%rax);"                                     \
                "mov %%rdx, 0x18(%%rax);"                                     \
                "mov %%rdi, 0x20(%%rax);"                                     \
                "mov %%rsi, 0x28(%%rax);"                                     \
                "mov %%rsp, 0x30(%%rax);"                                     \
                "mov %%rbp, 0x38(%%rax);"                                     \
                "mov %%r8,  0x40(%%rax);"                                     \
                "mov %%r9,  0x48(%%rax);"                                     \
                "mov %%r10, 0x50(%%rax);"                                     \
                "mov %%r11, 0x58(%%rax);"                                     \
                "mov %%r12, 0x60(%%rax);"                                     \
                "mov %%r13, 0x68(%%rax);"                                     \
                "mov %%r14, 0x70(%%rax);"                                     \
                "mov %%r15, 0x78(%%rax);"                                     \
                                                                              \
                "lea %1, %%rsp;"                                              \
                                                                              \
                "mov %3, %%rdi;"                                              \
                "mov %%rax, %%rsi;"                                           \
                "call *%2;"                                                   \
                                                                              \
                "lea %0, %%rax;"                                              \
                "mov 0x08(%%rax), %%rbx;"                                     \
                "mov 0x10(%%rax), %%rcx;"                                     \
                "mov 0x18(%%rax), %%rdx;"                                     \
                "mov 0x20(%%rax), %%rdi;"                                     \
                "mov 0x28(%%rax), %%rsi;"                                     \
                "mov 0x30(%%rax), %%rsp;"                                     \
                "mov 0x38(%%rax), %%rbp;"                                     \
                "mov 0x40(%%rax), %%r8;"                                      \
                "mov 0x48(%%rax), %%r9;"                                      \
                "mov 0x50(%%rax), %%r10;"                                     \
                "mov 0x58(%%rax), %%r11;"                                     \
                "mov 0x60(%%rax), %%r12;"                                     \
                "mov 0x68(%%rax), %%r13;"                                     \
                "mov 0x70(%%rax), %%r14;"                                     \
                "mov 0x78(%%rax), %%r15;"                                     \
                "mov 0x00(%%rax), %%rax;"                                     \
                                                                              \
                "iretq;"                                                      \
                : "+m"(f), "+m"(stack[sizeof (stack)-8])                      \
                : "m"(funs[0]), "i"(NUM));                                    \
}

INTR_HANDLER (0)  INTR_HANDLER (1)  INTR_HANDLER (2)  INTR_HANDLER (3) 
INTR_HANDLER (4)  INTR_HANDLER (5)  INTR_HANDLER (6)  INTR_HANDLER (7) 
INTR_HANDLER (8)  INTR_HANDLER (9)  INTR_HANDLER (10) INTR_HANDLER (11) 
INTR_HANDLER (12) INTR_HANDLER (13) INTR_HANDLER (14) INTR_HANDLER (15)
INTR_HANDLER (16) INTR_HANDLER (17) INTR_HANDLER (18) INTR_HANDLER (19)
INTR_HANDLER (20) INTR_HANDLER (21) INTR_HANDLER (22) INTR_HANDLER (23)
INTR_HANDLER (24) INTR_HANDLER (25) INTR_HANDLER (26) INTR_HANDLER (27)
INTR_HANDLER (28) INTR_HANDLER (29) INTR_HANDLER (30) INTR_HANDLER (31)

static struct idt_entry idt_entries[32] ALIGNED (0x10);

static const struct idtr idtr ALIGNED (0x10) = {
  .limit = sizeof (idt_entries) - 1,
  .offset = (uintptr_t) &idt_entries[0]
};

bool
interrupts_init (void)
{
  for (int i = 0; i < 32; ++i)
    {
    }
  for (int i = 0; i < 32; ++i)
    funs[i] = &intr_default_handler;

#define INTR_HANDLER_SET_ADDR(NUM)                                            \
      do                                                                      \
        {                                                                     \
          uintptr_t fun = (uintptr_t) &CASSERT_CONCAT (intr_handler_, NUM);   \
          idt_entries[NUM].offset_low = (fun & 0x000000000000FFFFull) >> 0;   \
          idt_entries[NUM].selector = 8;                                      \
          idt_entries[NUM].zero1 = 0;                                         \
          idt_entries[NUM].type = IDT_INTERRUPT_GATE | IDT_DPL_0 |            \
                                  IDT_ENTRY_PRESENT;                          \
          idt_entries[NUM].offset_mid = (fun & 0x00000000FFFF0000ull) >> 16;  \
          idt_entries[NUM].offset_high = (fun & 0xFFFFFFFF00000000ull) >> 32; \
          idt_entries[NUM].zero2    = 0;                                      \
        }                                                                     \
      while (false)

  INTR_HANDLER_SET_ADDR (0);  INTR_HANDLER_SET_ADDR (1);
  INTR_HANDLER_SET_ADDR (2);  INTR_HANDLER_SET_ADDR (3);
  INTR_HANDLER_SET_ADDR (4);  INTR_HANDLER_SET_ADDR (5);
  INTR_HANDLER_SET_ADDR (6);  INTR_HANDLER_SET_ADDR (7);
  INTR_HANDLER_SET_ADDR (8);  INTR_HANDLER_SET_ADDR (9);
  INTR_HANDLER_SET_ADDR (10); INTR_HANDLER_SET_ADDR (11);
  INTR_HANDLER_SET_ADDR (12); INTR_HANDLER_SET_ADDR (13);
  INTR_HANDLER_SET_ADDR (14); INTR_HANDLER_SET_ADDR (15);
  INTR_HANDLER_SET_ADDR (16); INTR_HANDLER_SET_ADDR (17);
  INTR_HANDLER_SET_ADDR (18); INTR_HANDLER_SET_ADDR (19);
  INTR_HANDLER_SET_ADDR (20); INTR_HANDLER_SET_ADDR (21);
  INTR_HANDLER_SET_ADDR (22); INTR_HANDLER_SET_ADDR (23);
  INTR_HANDLER_SET_ADDR (24); INTR_HANDLER_SET_ADDR (25);
  INTR_HANDLER_SET_ADDR (26); INTR_HANDLER_SET_ADDR (27);
  INTR_HANDLER_SET_ADDR (28); INTR_HANDLER_SET_ADDR (29);
  INTR_HANDLER_SET_ADDR (30); INTR_HANDLER_SET_ADDR (31);

  asm volatile ("lidtq %0" :: "m"(idtr));
  return true;
}

void
interrupts_set_handler (int num, intr_handler_fun *fun)
{
  ASSERT (num >= 0 && num <=32);
  funs[num] = fun;
  asm volatile ("lidtq %0" :: "m"(idtr));
}
