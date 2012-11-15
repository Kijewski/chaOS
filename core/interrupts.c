#include "interrupts.h"
#include "kmain.h"
#include "syscalls.h"

#include <assert.h>
#include <string.h>
#include <common/attributes.h>
#include <common/crX.h>
#include <common/round.h>
#include <inttypes.h>
#include <stdarg.h>

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

struct idtr
{
  uint16_t limit;
  uint64_t offset;
} PACKED;

static intr_handler_fun *interrupt_funs[256];

static const char *exception_name[20] = {
  "Divide by zero",
  "Debug",
  "Non-maskable Interrupt",
  "Breakpoint",
  "Overflow",

  "Bound range",
  "Invalid Opcode",
  "Device not available",
  "Double Fault",
  "Coprocessor Segment Overrun",

  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection Fault",
  "Page Fault",

  "Reserved (#15)",
  "x87 Floating-Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
};

static void
intr_debug_frame (int num, struct interrupt_frame *f)
{
  videoram_printf ("\n\n\e%c "
                   "              Got interrupt 0x%02hhX               \n",
                   COLOR_ERROR, num);
  if (num >= 0 && num < 20)
    videoram_printf ("\e%c  %44s  \n", COLOR_ERROR, exception_name[num]);
  videoram_printf ("\e%c RAX: %016" PRIxPTR "   "
                   " RBX: %016" PRIxPTR " \n"
                   " RCX: %016" PRIxPTR "   "
                   " RDX: %016" PRIxPTR " \n"
                   " RSI: %016" PRIxPTR "   "
                   " RDI: %016" PRIxPTR " \n"
                   " RBP: %016" PRIxPTR "   "
                   " RSP: %016" PRIxPTR " \n"
                   " R8 : %016" PRIxPTR "   "
                   " R9 : %016" PRIxPTR " \n"
                   " R10: %016" PRIxPTR "   "
                   " R11: %016" PRIxPTR " \n"
                   " R12: %016" PRIxPTR "   "
                   " R13: %016" PRIxPTR " \n"
                   " R14: %016" PRIxPTR "   "
                   " R15: %016" PRIxPTR " \n"
                   " CR2: %016" PRIxPTR "   "
                   " RIP: %016" PRIxPTR " \n"
                   " CR0: %016" PRIxPTR "   "
                   " CR3: %016" PRIxPTR " \n"
                   " CR4: %016" PRIxPTR "   "
                   "EFER: %016" PRIxPTR " \n"
                   "             FLAGS: %016" PRIxPTR "            \n\n",
                   COLOR_ERROR,
                   f->rax, f->rbx, f->rcx, f->rdx, f->rsi, f->rdi, f->rbp,
                   f->rsp, f->r8, f->r9, f->r10, f->r11, f->r12, f->r13, f->r14,
                   f->r15, cr2_read (), *(uintptr_t *)f->rsp, cr0_read (),
                   cr3_read (), cr4_read (), msr_read (MSR_EFER),
                   f->flags);
}

static void
intr_default_handler (int num, struct interrupt_frame *f)
{
  intr_debug_frame (num, f);
  khalt ();
}

struct interrupt_frame *
intr_handler (int num, struct interrupt_frame *f)
{
  if (interrupt_funs[num])
    interrupt_funs[num] (num, f);
  else
    intr_default_handler (num, f);
  return f;
}

static const struct idtr idtr = {
  .limit = sizeof (idt_entries) - 1,
  .offset = (uintptr_t) &idt_entries[0]
};

static void
intr_syscall (int num UNUSED, struct interrupt_frame *f)
{
  f->rax = syscall_handle ((unsigned) f->rax, f);
}

bool
interrupts_init (void)
{
  asm volatile ("lidtq %0" :: "m"(idtr));
  interrupts_set_handler (INT_SPECIAL_DEBUG, intr_debug_frame);
  interrupts_set_handler (INT_SYSCALL, intr_syscall);
  return true;
}

void
interrupts_set_handler (int num, intr_handler_fun *fun)
{
  ASSERT (num >= 0 && num < (int) ARRAY_LEN (interrupt_funs));
  interrupt_funs[num] = fun;
}

void
interrupts_finit (void)
{
  static const struct idtr no_idtr = { .limit = 0, .offset = 0 };
  asm volatile ("cli;"
                "lidtq %0" :: "m"(no_idtr));
}
