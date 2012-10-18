#ifndef __COMMON_CPUID_H
#define __COMMON_CPUID_H

#include "attributes.h"
#include <stdint.h>

struct cpuid_basic_info
{
  uint32_t max_cpuid;
  char vendor_string[12];
} PACKED;

struct cpuid_processor_brand_string
{
  char string[3*4*4];
} PACKED;

enum
{
  CPUID_FEAT_ECX_SSE3         = 1 << 0,
  CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
  CPUID_FEAT_ECX_DTES64       = 1 << 2,
  CPUID_FEAT_ECX_MONITOR      = 1 << 3,
  CPUID_FEAT_ECX_DS_CPL       = 1 << 4,
  CPUID_FEAT_ECX_VMX          = 1 << 5,
  CPUID_FEAT_ECX_SMX          = 1 << 6,
  CPUID_FEAT_ECX_EST          = 1 << 7,
  CPUID_FEAT_ECX_TM2          = 1 << 8,
  CPUID_FEAT_ECX_SSSE3        = 1 << 9,
  CPUID_FEAT_ECX_CID          = 1 << 10,
  CPUID_FEAT_ECX_FMA          = 1 << 12,
  CPUID_FEAT_ECX_CX16         = 1 << 13,
  CPUID_FEAT_ECX_ETPRD        = 1 << 14,
  CPUID_FEAT_ECX_PDCM         = 1 << 15,
  CPUID_FEAT_ECX_DCA          = 1 << 18,
  CPUID_FEAT_ECX_SSE4_1       = 1 << 19,
  CPUID_FEAT_ECX_SSE4_2       = 1 << 20,
  CPUID_FEAT_ECX_x2APIC       = 1 << 21,
  CPUID_FEAT_ECX_MOVBE        = 1 << 22,
  CPUID_FEAT_ECX_POPCNT       = 1 << 23,
  CPUID_FEAT_ECX_AES          = 1 << 25,
  CPUID_FEAT_ECX_XSAVE        = 1 << 26,
  CPUID_FEAT_ECX_OSXSAVE      = 1 << 27,
  CPUID_FEAT_ECX_AVX          = 1 << 28,

  CPUID_FEAT_EDX_FPU          = 1ull << (0 + 32),
  CPUID_FEAT_EDX_VME          = 1ull << (1 + 32),
  CPUID_FEAT_EDX_DE           = 1ull << (2 + 32),
  CPUID_FEAT_EDX_PSE          = 1ull << (3 + 32),
  CPUID_FEAT_EDX_TSC          = 1ull << (4 + 32),
  CPUID_FEAT_EDX_MSR          = 1ull << (5 + 32),
  CPUID_FEAT_EDX_PAE          = 1ull << (6 + 32),
  CPUID_FEAT_EDX_MCE          = 1ull << (7 + 32),
  CPUID_FEAT_EDX_CX8          = 1ull << (8 + 32),
  CPUID_FEAT_EDX_APIC         = 1ull << (9 + 32),
  CPUID_FEAT_EDX_SEP          = 1ull << (11 + 32),
  CPUID_FEAT_EDX_MTRR         = 1ull << (12 + 32),
  CPUID_FEAT_EDX_PGE          = 1ull << (13 + 32),
  CPUID_FEAT_EDX_MCA          = 1ull << (14 + 32),
  CPUID_FEAT_EDX_CMOV         = 1ull << (15 + 32),
  CPUID_FEAT_EDX_PAT          = 1ull << (16 + 32),
  CPUID_FEAT_EDX_PSE36        = 1ull << (17 + 32),
  CPUID_FEAT_EDX_PSN          = 1ull << (18 + 32),
  CPUID_FEAT_EDX_CLF          = 1ull << (19 + 32),
  CPUID_FEAT_EDX_DTES         = 1ull << (21 + 32),
  CPUID_FEAT_EDX_ACPI         = 1ull << (22 + 32),
  CPUID_FEAT_EDX_MMX          = 1ull << (23 + 32),
  CPUID_FEAT_EDX_FXSR         = 1ull << (24 + 32),
  CPUID_FEAT_EDX_SSE          = 1ull << (25 + 32),
  CPUID_FEAT_EDX_SSE2         = 1ull << (26 + 32),
  CPUID_FEAT_EDX_SS           = 1ull << (27 + 32),
  CPUID_FEAT_EDX_HTT          = 1ull << (28 + 32),
  CPUID_FEAT_EDX_TM1          = 1ull << (29 + 32),
  CPUID_FEAT_EDX_IA64         = 1ull << (30 + 32),
  CPUID_FEAT_EDX_PBE          = 1ull << (31 + 32),
};

void cpuid_basic_info (struct cpuid_basic_info *);
void cpuid_processor_brand_string (struct cpuid_processor_brand_string *);
uint64_t cpuid_features (void);

#endif
