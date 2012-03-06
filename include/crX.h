#ifndef CR_X_H__
#define CR_X_H__

#include <stdint.h>
#include <attributes.h>

enum
{
  CR0_PR = 1 <<  0,
  CR0_MP = 1 <<  1,
  CR0_EM = 1 <<  2,
  CR0_TS = 1 <<  3,
  CR0_ET = 1 <<  4,
  CR0_NE = 1 <<  5,
  CR0_WP = 1 << 16,
  CR0_AM = 1 << 18,
  CR0_NW = 1 << 29,
  CR0_CD = 1 << 30,
  CR0_PG = 1 << 31,
};

INLINE_ONLY (void)
cr0_set_to (int64_t value)
{
  asm volatile ("mov %0, %%cr0" :: "a"(value));
}

INLINE_ONLY (void)
cr0_set_reset (int64_t set, int64_t reset)
{
  register int64_t rax asm ("rax");
  asm volatile ("mov %%cr0, %0" : "=a"(rax));
  rax |= set;
  rax &= ~reset;
  asm volatile ("mov %0, %%cr0" :: "a"(rax));
}

INLINE_ONLY (void)
cr0_set_bits (int64_t value)
{
  register int64_t rax asm ("rax");
  asm volatile ("mov %%cr0, %0" : "=a"(rax));
  rax |= value;
  asm volatile ("mov %0, %%cr0" :: "a"(rax));
}

INLINE_ONLY (void)
cr0_mask_bits (int64_t value)
{
  register int64_t rax asm ("rax");
  asm volatile ("mov %%cr0, %0" : "=a"(rax));
  rax &= value;
  asm volatile ("mov %0, %%cr0" :: "a"(rax));
}

INLINE_ONLY (void)
cr0_reset_bits (int64_t value)
{
  register int64_t rax asm ("rax");
  asm volatile ("mov %%cr0, %0" : "=a"(rax));
  rax &= ~value;
  asm volatile ("mov %0, %%cr0" :: "a"(rax));
}

INLINE_ONLY (intptr_t)
cr2_read (void)
{
  register int64_t rax asm ("rax");
  asm volatile ("mov %%cr2, %0" : "=a"(rax));
  return rax;
}

INLINE_ONLY (void)
cr3_set_to (int64_t value)
{
  asm volatile ("mov %0, %%cr3" :: "a"(value));
}

#endif
