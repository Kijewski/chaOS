// TODO: translate whole file to (n)asm

#include "interrupts.h"
#include "kmain.h"
#include <assert.h>
#include <string.h>
#include <attributes.h>
#include <crX.h>
#include <round.h>

#pragma GCC optimize "omit-frame-pointer", "Os"

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

static void
intr_debug_frame (int num, struct interrupt_frame *f)
{
  videoram_puts ("\n Got interrupt 0x", COLOR_ERROR);
  videoram_put_hex (num, COLOR_ERROR);
  videoram_puts ("! \n", COLOR_ERROR);

  videoram_puts ( "\n RAX: ", 7); videoram_put_all_hex (f->rax, 7);
  videoram_puts ("    RBX: ", 7); videoram_put_all_hex (f->rbx, 7);
  videoram_puts (" \n RCX: ", 7); videoram_put_all_hex (f->rcx, 7);
  videoram_puts ("    RDX: ", 7); videoram_put_all_hex (f->rdx, 7);

  videoram_puts (" \n RSI: ", 7); videoram_put_all_hex (f->rsi, 7);
  videoram_puts ("    RDI: ", 7); videoram_put_all_hex (f->rdi, 7);
  videoram_puts (" \n RBP: ", 7); videoram_put_all_hex (f->rbp, 7);
  videoram_puts ("    RSP: ", 7); videoram_put_all_hex (f->rsp, 7);

  videoram_puts (" \n R8 : ", 7); videoram_put_all_hex (f->r8 , 7);
  videoram_puts ("    R9 : ", 7); videoram_put_all_hex (f->r9 , 7);
  videoram_puts (" \n R10: ", 7); videoram_put_all_hex (f->r10, 7);
  videoram_puts ("    R11: ", 7); videoram_put_all_hex (f->r11, 7);

  videoram_puts (" \n R12: ", 7); videoram_put_all_hex (f->r12, 7);
  videoram_puts ("    R13: ", 7); videoram_put_all_hex (f->r13, 7);
  videoram_puts (" \n R14: ", 7); videoram_put_all_hex (f->r14, 7);
  videoram_puts ("    R15: ", 7); videoram_put_all_hex (f->r15, 7);

  videoram_puts (" \n\n", 7);
  videoram_puts (   " CR2: ", 7); videoram_put_all_hex (cr2_read (), 7);
  videoram_puts ("    RIP: ", 7); videoram_put_all_hex (*(uintptr_t *)f->rsp,7);
  videoram_puts (" \n\n", 7);

  videoram_puts (   " CR0: ", 7); videoram_put_all_hex (cr0_read (), 7);
  videoram_puts ("    CR3: ", 7); videoram_put_all_hex (cr3_read (), 7);
  videoram_puts (" \n CR4: ", 7); videoram_put_all_hex (cr4_read (), 7);
  videoram_puts ("   EFER: ", 7); videoram_put_all_hex (msr_read (MSR_EFER), 7);

  videoram_puts (" \n", 7);
}

static void
intr_default_handler (int num, struct interrupt_frame *f)
{
  intr_debug_frame (num, f);
  khalt ();
}

void intr_handler (int num, struct interrupt_frame *f);
void
intr_handler (int num, struct interrupt_frame *f)
{
  if (interrupt_funs[num])
    interrupt_funs[num] (num, f);
  else
    intr_default_handler (num, f);

  asm volatile ("mov %0, %%rax;"
                "mov 0x08(%%rax), %%rbx;"
                "mov 0x10(%%rax), %%rcx;"
                "mov 0x18(%%rax), %%rdx;"
                "mov 0x20(%%rax), %%rdi;"
                "mov 0x28(%%rax), %%rsi;"
                "mov 0x30(%%rax), %%rsp;"
                "mov 0x38(%%rax), %%rbp;"
                "mov 0x40(%%rax), %%r8;"
                "mov 0x48(%%rax), %%r9;"
                "mov 0x50(%%rax), %%r10;"
                "mov 0x58(%%rax), %%r11;"
                "mov 0x60(%%rax), %%r12;"
                "mov 0x68(%%rax), %%r13;"
                "mov 0x70(%%rax), %%r14;"
                "mov 0x78(%%rax), %%r15;"
                "mov 0x00(%%rax), %%rax;"
                "iretq;" :: "m"(f));
  UNREACHABLE ();
}

void intr_handler_x (void);
void
intr_handler_x (void)
{
  asm volatile ("mov %rcx, 0x10(%rax);"
                "mov %rdx, 0x18(%rax);"
                "mov %rdi, 0x20(%rax);"
                "mov %rsi, 0x28(%rax);"
                "mov %rsp, 0x30(%rax);"
                "mov %rbp, 0x38(%rax);"
                "mov %r8,  0x40(%rax);"
                "mov %r9,  0x48(%rax);"
                "mov %r10, 0x50(%rax);"
                "mov %r11, 0x58(%rax);"
                "mov %r12, 0x60(%rax);"
                "mov %r13, 0x68(%rax);"
                "mov %r14, 0x70(%rax);"
                "mov %r15, 0x78(%rax);"

                "movzx %bl, %edi;"
                "mov %rax, %rsi;"
                "call intr_handler;");
  UNREACHABLE ();
}

#define INTR_HANDLER(NUM)                                                     \
GLOBAL_CASSERT (ARRAY_LEN (interrupt_funs) >= NUM);                           \
void CASSERT_CONCAT_ (intr_handler_, NUM) (void);                             \
void ALIGNED(0x10) CASSERT_CONCAT_ (intr_handler_, NUM) (void)                \
{                                                                             \
  static struct interrupt_frame f;                                            \
  asm volatile ("mov %%rax, %0;"                                              \
                "lea %0, %%rax;"                                              \
                "mov %%rbx, 0x08(%%rax);"                                     \
                "mov %1, %%rbx;"                                              \
                "jmp intr_handler_x;"                                         \
                : "+m"(f)                                                     \
                : "i"(NUM));                                                  \
  UNREACHABLE ();                                                             \
}

INTR_HANDLER (0)   INTR_HANDLER (1)   INTR_HANDLER (2)   INTR_HANDLER (3) 
INTR_HANDLER (4)   INTR_HANDLER (5)   INTR_HANDLER (6)   INTR_HANDLER (7) 
INTR_HANDLER (8)   INTR_HANDLER (9)   INTR_HANDLER (10)  INTR_HANDLER (11) 
INTR_HANDLER (12)  INTR_HANDLER (13)  INTR_HANDLER (14)  INTR_HANDLER (15)
INTR_HANDLER (16)  INTR_HANDLER (17)  INTR_HANDLER (18)  INTR_HANDLER (19)
INTR_HANDLER (20)  INTR_HANDLER (21)  INTR_HANDLER (22)  INTR_HANDLER (23)
INTR_HANDLER (24)  INTR_HANDLER (25)  INTR_HANDLER (26)  INTR_HANDLER (27)
INTR_HANDLER (28)  INTR_HANDLER (29)  INTR_HANDLER (30)  INTR_HANDLER (31)
INTR_HANDLER (32)  INTR_HANDLER (33)  INTR_HANDLER (34)  INTR_HANDLER (35)
INTR_HANDLER (36)  INTR_HANDLER (37)  INTR_HANDLER (38)  INTR_HANDLER (39)
INTR_HANDLER (40)  INTR_HANDLER (41)  INTR_HANDLER (42)  INTR_HANDLER (43)
INTR_HANDLER (44)  INTR_HANDLER (45)  INTR_HANDLER (46)  INTR_HANDLER (47)
INTR_HANDLER (48)  INTR_HANDLER (49)  INTR_HANDLER (50)  INTR_HANDLER (51)
INTR_HANDLER (52)  INTR_HANDLER (53)  INTR_HANDLER (54)  INTR_HANDLER (55)
INTR_HANDLER (56)  INTR_HANDLER (57)  INTR_HANDLER (58)  INTR_HANDLER (59)
INTR_HANDLER (60)  INTR_HANDLER (61)  INTR_HANDLER (62)  INTR_HANDLER (63)
INTR_HANDLER (64)  INTR_HANDLER (65)  INTR_HANDLER (66)  INTR_HANDLER (67)
INTR_HANDLER (68)  INTR_HANDLER (69)  INTR_HANDLER (70)  INTR_HANDLER (71)
INTR_HANDLER (72)  INTR_HANDLER (73)  INTR_HANDLER (74)  INTR_HANDLER (75)
INTR_HANDLER (76)  INTR_HANDLER (77)  INTR_HANDLER (78)  INTR_HANDLER (79)
INTR_HANDLER (80)  INTR_HANDLER (81)  INTR_HANDLER (82)  INTR_HANDLER (83)
INTR_HANDLER (84)  INTR_HANDLER (85)  INTR_HANDLER (86)  INTR_HANDLER (87)
INTR_HANDLER (88)  INTR_HANDLER (89)  INTR_HANDLER (90)  INTR_HANDLER (91)
INTR_HANDLER (92)  INTR_HANDLER (93)  INTR_HANDLER (94)  INTR_HANDLER (95)
INTR_HANDLER (96)  INTR_HANDLER (97)  INTR_HANDLER (98)  INTR_HANDLER (99)
INTR_HANDLER (100) INTR_HANDLER (101) INTR_HANDLER (102) INTR_HANDLER (103)
INTR_HANDLER (104) INTR_HANDLER (105) INTR_HANDLER (106) INTR_HANDLER (107)
INTR_HANDLER (108) INTR_HANDLER (109) INTR_HANDLER (110) INTR_HANDLER (111)
INTR_HANDLER (112) INTR_HANDLER (113) INTR_HANDLER (114) INTR_HANDLER (115)
INTR_HANDLER (116) INTR_HANDLER (117) INTR_HANDLER (118) INTR_HANDLER (119)
INTR_HANDLER (120) INTR_HANDLER (121) INTR_HANDLER (122) INTR_HANDLER (123)
INTR_HANDLER (124) INTR_HANDLER (125) INTR_HANDLER (126) INTR_HANDLER (127)
INTR_HANDLER (128) INTR_HANDLER (129) INTR_HANDLER (130) INTR_HANDLER (131)
INTR_HANDLER (132) INTR_HANDLER (133) INTR_HANDLER (134) INTR_HANDLER (135)
INTR_HANDLER (136) INTR_HANDLER (137) INTR_HANDLER (138) INTR_HANDLER (139)
INTR_HANDLER (140) INTR_HANDLER (141) INTR_HANDLER (142) INTR_HANDLER (143)
INTR_HANDLER (144) INTR_HANDLER (145) INTR_HANDLER (146) INTR_HANDLER (147)
INTR_HANDLER (148) INTR_HANDLER (149) INTR_HANDLER (150) INTR_HANDLER (151)
INTR_HANDLER (152) INTR_HANDLER (153) INTR_HANDLER (154) INTR_HANDLER (155)
INTR_HANDLER (156) INTR_HANDLER (157) INTR_HANDLER (158) INTR_HANDLER (159)
INTR_HANDLER (160) INTR_HANDLER (161) INTR_HANDLER (162) INTR_HANDLER (163)
INTR_HANDLER (164) INTR_HANDLER (165) INTR_HANDLER (166) INTR_HANDLER (167)
INTR_HANDLER (168) INTR_HANDLER (169) INTR_HANDLER (170) INTR_HANDLER (171)
INTR_HANDLER (172) INTR_HANDLER (173) INTR_HANDLER (174) INTR_HANDLER (175)
INTR_HANDLER (176) INTR_HANDLER (177) INTR_HANDLER (178) INTR_HANDLER (179)
INTR_HANDLER (180) INTR_HANDLER (181) INTR_HANDLER (182) INTR_HANDLER (183)
INTR_HANDLER (184) INTR_HANDLER (185) INTR_HANDLER (186) INTR_HANDLER (187)
INTR_HANDLER (188) INTR_HANDLER (189) INTR_HANDLER (190) INTR_HANDLER (191)
INTR_HANDLER (192) INTR_HANDLER (193) INTR_HANDLER (194) INTR_HANDLER (195)
INTR_HANDLER (196) INTR_HANDLER (197) INTR_HANDLER (198) INTR_HANDLER (199)
INTR_HANDLER (200) INTR_HANDLER (201) INTR_HANDLER (202) INTR_HANDLER (203)
INTR_HANDLER (204) INTR_HANDLER (205) INTR_HANDLER (206) INTR_HANDLER (207)
INTR_HANDLER (208) INTR_HANDLER (209) INTR_HANDLER (210) INTR_HANDLER (211)
INTR_HANDLER (212) INTR_HANDLER (213) INTR_HANDLER (214) INTR_HANDLER (215)
INTR_HANDLER (216) INTR_HANDLER (217) INTR_HANDLER (218) INTR_HANDLER (219)
INTR_HANDLER (220) INTR_HANDLER (221) INTR_HANDLER (222) INTR_HANDLER (223)
INTR_HANDLER (224) INTR_HANDLER (225) INTR_HANDLER (226) INTR_HANDLER (227)
INTR_HANDLER (228) INTR_HANDLER (229) INTR_HANDLER (230) INTR_HANDLER (231)
INTR_HANDLER (232) INTR_HANDLER (233) INTR_HANDLER (234) INTR_HANDLER (235)
INTR_HANDLER (236) INTR_HANDLER (237) INTR_HANDLER (238) INTR_HANDLER (239)
INTR_HANDLER (240) INTR_HANDLER (241) INTR_HANDLER (242) INTR_HANDLER (243)
INTR_HANDLER (244) INTR_HANDLER (245) INTR_HANDLER (246) INTR_HANDLER (247)
INTR_HANDLER (248) INTR_HANDLER (249) INTR_HANDLER (250) INTR_HANDLER (251)
INTR_HANDLER (252) INTR_HANDLER (253) INTR_HANDLER (254) INTR_HANDLER (255)

static const struct idtr idtr = {
  .limit = sizeof (idt_entries) - 1,
  .offset = (uintptr_t) &idt_entries[0]
};

bool
interrupts_init (void)
{
  asm volatile ("lidtq %0" :: "m"(idtr));
  interrupts_set_handler (INT_SPECIAL_DEBUG, intr_debug_frame);
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
