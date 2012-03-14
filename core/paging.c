#include "paging.h"
#include "e820.h"
#include "kmain.h"

#include <spinlock.h>
#include <stdint.h>
#include <string.h>
#include <kernel.h>
#include <round.h>

#include <spinlock.h>
#include <list.h>

typedef uint8_t block[4096];

static spinlock flat_memory_freemap_lock;
static struct list flat_memory_free_pages;

struct flat_memory_free_entry
{
  struct list_elem elem;
  block *data;
};

static void
init_flat_freemap (void)
{
  auto void use_block (block *data);
  static block first_block;

  spinlock_init (&flat_memory_freemap_lock);
  list_init (&flat_memory_free_pages);

  struct list free_refs;
  list_init (&free_refs);

  auto void use_block (block *data) {
    for (unsigned pos = 0; pos < 4096;
         pos += sizeof (struct flat_memory_free_entry))
      {
        struct flat_memory_free_entry *b = (void *) &data[0][pos];
        list_push_back (&free_refs, &b->elem);
      }
  }
  use_block (&first_block);

  for (const struct e820_ref *mem = E820_BASE; mem; mem = e820_next (mem))
    {
      if (mem->entry.type != E820_MEMORY)
        continue;

      block *start, *end;
      start = (void *) round_up_pow2 (MAX (mem->entry.base_addr,
                                           (uintptr_t) &_kernel_memory_end),
                                      12);
      end = (void *) round_down_pow2 (mem->entry.base_addr + mem->entry.length,
                                      12);
      for (block *i = start; i < end; ++i)
        {
          struct list_elem *e;
          struct flat_memory_free_entry *ee;
          if (!list_is_empty (&free_refs))
            e = list_pop_front (&free_refs);
          else
            {
              e = list_pop_front (&flat_memory_free_pages);
              ee = list_entry (e, *ee, elem);
              use_block (ee->data);
              e = list_pop_front (&free_refs);
            }
          ee = list_entry (e, *ee, elem);
          ee->data = i;
          list_push_back (&flat_memory_free_pages, &ee->elem);
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
