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

static spinlock freemap_lock = SPINLOCK_INITIALIZER;

struct freemap
{
  uint64_t map;
  void *base;
  struct freemap *submap;
};

static struct freemap *freemap;

static void
freemap_add_layer (struct freemap *layer_base, size_t layer_item_count)
{
  struct freemap *layer = layer_base + layer_item_count;
  while (layer_item_count > 0)
    {
      size_t layer_len = MIN (layer_item_count, 64u);
      layer_item_count -= layer_len;
      layer_base += layer_len;

      layer->map = ~(int64_t) (((int128_t) 1 << layer_item_count) - 1);
      layer->base = NULL;
      layer->submap = layer_base;

      ++layer;
    }
}

static void
create_freemap (void)
{
  struct freemap *layer = (void *) &_kernel_memory_end;
  struct freemap *layer_base = layer;
  size_t layer_item_count = 0;

  for (const struct e820_ref *ref = E820_BASE; ref; ref = e820_next (ref))
    {
      if (ref->entry.type != E820_MEMORY || ref->entry.base_addr < 1024*1024)
        continue;

      uint64_t base = ref->entry.base_addr;
      uint64_t length = ref->entry.length;
      if (base % 4096 != 0)
        {
          length -= base % 4096;
          base += 4096 - base % 4096;
        }

      length /= 4096;
      layer_item_count += length;

      while (length--)
        {
          layer->map = 0xCCCCCCCCCCCCCCCC;
          layer->base = (void *) base;
          layer->submap = NULL;
          ++layer;

          base += 4096;
        }
    }
    
  while (layer_item_count > 1)
    {
      freemap_add_layer (layer_base, layer_item_count);
      layer_item_count = (layer_item_count+63) / 64;
      layer_base = layer_base + layer_item_count;
    }
  freemap = layer_base;
}

static void
mark_used_items_in_freemap (void)
{
  // TODO
}

bool
paging_init (void)
{
  // FIXME: there is a bug in create_freemap() or freemap_add_layer() that
  //        crashes the system, if more than 200±$RANDOM MB of RAM are present!

  //create_freemap ();
  //mark_used_items_in_freemap ();

  return true;
}

void
paging_enable (void)
{
  // TODO

  // With enabling paging the current stack becomes inaccessible
  static uintptr_t stack[0x1000];
  asm volatile ("mov %0, %%rax;"
                "mov %%rax, %%rsp;"
                "xor %%rbp, %%rbp;"
                "call kstart;" :: "i"(&stack[0x1000]) : "memory");
  khalt ();
}
