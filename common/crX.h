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

enum
{
  CR4_VME        = 1 <<  0,
  CR4_PVI        = 1 <<  1,
  CR4_TSD        = 1 <<  2,
  CR4_DE         = 1 <<  3,
  CR4_PSE        = 1 <<  4,
  CR4_PAE        = 1 <<  5,
  CR4_MCE        = 1 <<  6,
  CR4_PGE        = 1 <<  7,
  CR4_PCE        = 1 <<  8,
  CR4_OSFXSR     = 1 <<  9,
  CR4_OSXMMEXCPT = 1 << 10,
  CR4_VMXE       = 1 << 13,
  CR4_SMXE       = 1 << 14,
  CR4_PCIDE      = 1 << 17,
  CR4_OSXSAVE    = 1 << 18,
};

enum
{
  EFER_SCE   = 1 <<  0,
  EFER_LME   = 1 <<  8,
  EFER_LMA   = 1 << 10,
  EFER_NXE   = 1 << 11,
  EFER_FFXSR = 1 << 14,
};

enum
{
  MSR_EFER = 0xC0000080,
};

// CR0

static inline uint64_t
cr0_set_to (int64_t value)
{
  asm volatile ("mov %0, %%cr0" :: "a"(value));
  return value;
}

static inline uint64_t
cr0_set_reset (int64_t set, int64_t reset)
{
  register int64_t rax;
  asm volatile ("mov %%cr0, %0" : "=r"(rax));
  rax |= set;
  rax &= ~reset;
  asm volatile ("mov %0, %%cr0" :: "r"(rax));
  return rax;
}

static inline int64_t
cr0_set_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr0, %0" : "=r"(rax));
  rax |= value;
  asm volatile ("mov %0, %%cr0" :: "r"(rax));
  return rax;
}

static inline int64_t
cr0_mask_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr0, %0" : "=r"(rax));
  rax &= value;
  asm volatile ("mov %0, %%cr0" :: "r"(rax));
  return rax;
}

static inline int64_t
cr0_reset_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr0, %0" : "=r"(rax));
  rax &= ~value;
  asm volatile ("mov %0, %%cr0" :: "r"(rax));
  return rax;
}

static inline int64_t
cr0_read (void)
{
  register int64_t rax;
  asm volatile ("mov %%cr0, %0" : "=r"(rax));
  return rax;
}

// CR2

static inline intptr_t
cr2_read (void)
{
  register int64_t rax;
  asm volatile ("mov %%cr2, %0" : "=r"(rax));
  return rax;
}

// CR3

static inline int64_t
cr3_read (void)
{
  register int64_t rax;
  asm volatile ("mov %%cr3, %0" : "=r"(rax));
  return rax;
}

static inline void
cr3_set_to (int64_t value)
{
  asm volatile ("mov %0, %%cr3" :: "r"(value));
}

// CR4

static inline uint64_t
cr4_set_to (int64_t value)
{
  asm volatile ("mov %0, %%cr4" :: "r"(value));
  return value;
}

static inline uint64_t
cr4_set_reset (int64_t set, int64_t reset)
{
  register int64_t rax;
  asm volatile ("mov %%cr4, %0" : "=r"(rax));
  rax |= set;
  rax &= ~reset;
  asm volatile ("mov %0, %%cr4" :: "r"(rax));
  return rax;
}

static inline int64_t
cr4_set_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr4, %0" : "=r"(rax));
  rax |= value;
  asm volatile ("mov %0, %%cr4" :: "r"(rax));
  return rax;
}

static inline int64_t
cr4_mask_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr4, %0" : "=r"(rax));
  rax &= value;
  asm volatile ("mov %0, %%cr4" :: "r"(rax));
  return rax;
}

static inline int64_t
cr4_reset_bits (int64_t value)
{
  register int64_t rax;
  asm volatile ("mov %%cr4, %0" : "=r"(rax));
  rax &= ~value;
  asm volatile ("mov %0, %%cr4" :: "r"(rax));
  return rax;
}

static inline int64_t
cr4_read (void)
{
  register int64_t rax;
  asm volatile ("mov %%cr4, %0" : "=r"(rax));
  return rax;
}

// MSRs

static inline uint64_t
msr_set_reset (uint32_t msr, int64_t set, int64_t reset)
{
  register int32_t ecx asm ("rcx") = msr;
  register int64_t rax asm ("rax") = 0;
  if (set != ~reset)
    asm volatile ("rdmsr" : "=r"(rax) : "r"(ecx));
  if (set || reset)
    {
      rax |= set;
      rax &= ~reset;
      asm volatile ("wrmsr" :: "r"(rax), "r"(ecx));
    }
  return rax;
}

static inline uint64_t
msr_read (uint32_t msr)
{
  return msr_set_reset (msr, 0, 0);
}

static inline uint64_t
msr_set_to (uint32_t msr, int64_t value)
{
  return msr_set_reset (msr, value, ~value);
}

#endif
