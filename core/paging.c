#include "paging.h"
#include "e820.h"
#include "kmain.h"

#include <spinlock.h>
#include <bitmap.h>
#include <stdint.h>
#include <string.h>
#include <spinlock.h>
#include <kernel.h>
#include <round.h>

struct freemap_20
{
  uint64_t full12;
  uint64_t sub12[64];
};

struct freemap_28
{
  uint64_t full20;
  struct freemap_20 *sub20[64];
};

struct freemap_36
{
  uint64_t full28;
  struct freemap_28 *sub28[64];
};

static spinlock flat_memory_freemap_lock;
static struct freemap_36 flat_memory_freemap;

static void
init_flat_freemap (void)
{
  static struct freemap_28 first_28[16];
  static struct freemap_20 first_20[64 * ARRAY_LEN (first_28)];
  flat_memory_freemap_lock = SPINLOCK_INITIALIZER;

  first_28[0].full20 = 1; // first MB is reserved

  for (unsigned i = 0; i < ARRAY_LEN (first_28); ++i)
    {
      flat_memory_freemap.sub28[i] = &first_28[i];
      for (unsigned j = 0; j < 64; ++j)
        first_28[i].sub20[j] = &first_20[64*i + j];
    }

  first_20[0].full12 = -1ull;

  uintptr_t p = (uintptr_t) &_section_text_start;
  while (p < (uintptr_t) &_kernel_memory_end)
    {
      uint64_t b = 1;
      while (p < (uintptr_t) &_kernel_memory_end && b != 0)
        {
          first_20[p >> 20].full12 |= b;
          b <<= 1;
          p += 4096;
        }
    }
}

bool
paging_init (void)
{
  init_flat_freemap ();
  return true;
}

void
paging_enable (void)
{
  // TODO

  // With enabling paging the current stack becomes inaccessible
  static uintptr_t stack[0x4000];
  asm volatile ("mov %0, %%rsp;"
                "call kstart;" :: "i"(&stack[ARRAY_LEN (stack)-1]) : "memory");
  UNREACHABLE ();
}
