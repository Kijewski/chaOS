#include "paging.h"
#include "e820.h"
#include <spinlock.h>
#include <bitmap.h>
#include <stdint.h>
#include <string.h>
#include <spinlock.h>
#include <kernel.h>
#include <devices/videoram.h>

static spinlock freemap_lock = SPINLOCK_INITIALIZER;

INLINE_ONLY (uint64_t)
div_round_up (uint64_t n, uint64_t d)
{
  return (n+d-1) / d;
}

INLINE_ONLY (uint64_t)
round_down_18 (uint64_t ptr)
{
  return (uintptr_t) ptr & ~((1<<18) - 1);
}

INLINE_ONLY (uint64_t)
round_up_12 (uint64_t ptr)
{
  if (ptr & ~((1<<12) - 1))
    return (ptr | ((1<<12) - 1)) + 1;
  else
    return ptr;
}

struct freemap_18
{
  uint64_t map;
  void *base;
};

struct freemap_24
{
  uint64_t map;
  struct freemap_18 submaps[0];
};

struct freemap_30
{
  uint64_t map;
  struct freemap_24 submaps[0];
};

struct freemap_36
{
  uint64_t map;
  struct freemap_30 submaps[0];
};

struct freemap_42
{
  uint64_t map;
  struct freemap_36 submaps[0];
};

struct freemap_48
{
  uint64_t map;
  struct freemap_42 submaps[0];
};

static struct freemap_48 *freemap;

static uint64_t
get_memory_range_size (const struct e820_entry *entry)
{
  uint64_t base = entry->base_addr;
  uint64_t length = entry->length;

  if (entry->type != E820_MEMORY || base < 1024*1024)
    return 0;

  uint64_t size = round_down_18 (base + length) - round_up_12 (base);

  if (size >= (1<<18))
    return size;
  else
    return 0;
}

bool
paging_init (void)
{
  freemap = (void *) _kernel_memory_end;

  size_t total_memory = 0;
  for (const struct e820_ref *ref = E820_BASE; ref; ref = e820_next (ref))
    total_memory += get_memory_range_size (&ref->entry);

  uint64_t freemap_size =
      div_round_up (total_memory, 1ull<<18) * sizeof (struct freemap_18) +
      div_round_up (total_memory, 1ull<<24) * sizeof (struct freemap_24) +
      div_round_up (total_memory, 1ull<<30) * sizeof (struct freemap_30) +
      div_round_up (total_memory, 1ull<<36) * sizeof (struct freemap_36) +
      div_round_up (total_memory, 1ull<<42) * sizeof (struct freemap_42) +
      div_round_up (total_memory, 1ull<<48) * sizeof (struct freemap_48);
  freemap_size = round_up_12 (freemap_size);

  memset (freemap, -1u, freemap_size);

  // TODO: set up mapping, set available bits (recursively) to false
  //       Mind space claimed by the kernel and the freemap itself!
}
