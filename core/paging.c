#include "paging.h"
#include "e820.h"
#include "kmain.h"

#include <spinlock.h>
#include <list.h>
#include <stdint.h>
#include <string.h>
#include <kernel.h>
#include <round.h>
#include <assert.h>

static spinlock flat_memory_freemap_lock;
static struct list flat_memory_free_pages;
static struct list flat_memory_free_refs;

struct flat_memory_free_entry
{
  struct list_elem elem;
  uint8_t *data;
};

static void
flat_memory_free_refs_use_block (uint8_t *data)
{
  for (unsigned pos = 0; pos < 4096;
       pos += sizeof (struct flat_memory_free_entry))
    {
      struct flat_memory_free_entry *b = (void *) &data[pos];
      list_push_back (&flat_memory_free_refs, &b->elem);
    }
}

// need external locking!
static struct flat_memory_free_entry *
get_flat_memory_ref (void)
{
  struct list_elem *e;
  struct flat_memory_free_entry *ee;
  if (!list_is_empty (&flat_memory_free_refs))
    e = list_pop_front (&flat_memory_free_refs);
  else if (!list_is_empty (&flat_memory_free_pages))
    {
      e = list_pop_front (&flat_memory_free_pages);
      ee = list_entry (e, *ee, elem);
      flat_memory_free_refs_use_block (ee->data);
      e = list_pop_front (&flat_memory_free_refs);
    }
  else
    return NULL;
  ee = list_entry (e, *ee, elem);
  return ee;
}

// need external locking!
static bool
static_paging_return_page (void *page)
{
  struct flat_memory_free_entry *ee = get_flat_memory_ref ();
  if (!ee)
    return false;
  ee->data = page;
  list_push_back (&flat_memory_free_pages, &ee->elem);
  return true;
}

bool
paging_return_page (void *page)
{
  ASSERT (((uintptr_t) page & 0x0FFF) == 2);
  if (page == NULL)
    return true;
  spinlock_acquire (&flat_memory_freemap_lock);
  bool result = static_paging_return_page (page);
  spinlock_release (&flat_memory_freemap_lock);
  return result;
}

void *
paging_get_page (void)
{
  void *result = NULL;
  spinlock_acquire (&flat_memory_freemap_lock);
  if (!list_is_empty (&flat_memory_free_refs))
    {
      struct list_elem *e = list_pop_front (&flat_memory_free_pages);
      list_push_front (&flat_memory_free_refs, e);
      SWAP (result, list_entry (e, struct flat_memory_free_entry, elem)->data);
    }
  spinlock_release (&flat_memory_freemap_lock);
  return result;
}


static void
init_flat_freemap (void)
{
  static uint8_t first_block[4096];
  flat_memory_free_refs_use_block (&first_block[0]);

  for (const struct e820_ref *mem = E820_BASE; mem; mem = e820_next (mem))
    {
      if (mem->entry.type != E820_MEMORY)
        continue;

      uint8_t *start, *end;
      start = (void *) round_up_pow2 (MAX (mem->entry.base_addr,
                                           (uintptr_t) &_kernel_memory_end),
                                      12);
      end = (void *) round_down_pow2 (mem->entry.base_addr + mem->entry.length,
                                      12);
      for (uint8_t *i = start; i < end; i += 4096)
        {
          bool inserted UNUSED = static_paging_return_page (i);
          ASSERT (inserted);
        }
    }
}

bool
paging_init (void)
{
  spinlock_init (&flat_memory_freemap_lock);
  list_init (&flat_memory_free_pages);
  list_init (&flat_memory_free_refs);

  init_flat_freemap ();
  return true;
}

void
paging_enable (void)
{
  // TODO
}
