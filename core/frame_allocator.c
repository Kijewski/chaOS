#include "frame_allocator.h"
#include "e820.h"
#include "kmain.h"
#include "paging.h"

#include <common/list.h>
#include <common/round.h>
#include <assert.h>
#include <kernel.h>

static struct list free_frames;
static struct list free_entry_refs;
static uint64_t counter;
static bool initialized;

struct frame_ref
{
  struct list_elem elem;
  uint8_t *data;
};

static void
use_block (uint8_t *block)
{
  ASSERT (block != NULL);
  for (unsigned pos = 0; pos < 4096; pos += sizeof (struct frame_ref))
    {
      struct frame_ref *b = (void *) &block[pos];
      list_push_back (&free_entry_refs, &b->elem);
    }
}

static uint8_t *
get_frame (void)
{
  void *result = NULL;
  if (!list_is_empty (&free_frames))
    {
      struct list_elem *e = list_pop_front (&free_frames);
      list_push_front (&free_entry_refs, e);
      SWAP (result, list_entry (e, struct frame_ref, elem)->data);
      --counter;
    }
  return result;
}

static struct frame_ref *
get_ref (void)
{
  if (list_is_empty (&free_entry_refs))
    {
      uint8_t *block = get_frame ();
      if (!block)
        return NULL;
      paging_identity_map (block, PT_P|PT_RW|PT_PWT|PT_NX);
      use_block (block);
    }
  struct list_elem *e = list_pop_front (&free_entry_refs);
  struct frame_ref *ee = list_entry (e, *ee, elem);
  return ee;
}

static bool
return_frame (void *page)
{
  struct frame_ref *ee = get_ref ();
  if (ee)
    {
      ee->data = page;
      list_push_back (&free_frames, &ee->elem);
      ++counter;
    }
  else if (initialized)
    return false;
  else
    {
      paging_identity_map (page, PT_P|PT_RW|PT_PWT|PT_NX);
      use_block (page);
    }
  return true;
}

bool
frame_allocator_return (void *page)
{
  ASSERT (((uintptr_t) page & 0x0FFF) == 0);
  ASSERT (page != NULL);

  return return_frame (page);
}

void *
frame_allocator_get (void)
{
  return get_frame ();
}

static void
init_freemap (void)
{
  static uint8_t first_block[4096];
  use_block (&first_block[0]);

  for (const struct e820_ref *mem = e820_start (); mem; mem = e820_next (mem))
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
        return_frame (i);
    }
}

bool
frame_allocator_init (void)
{
  list_init (&free_frames);
  list_init (&free_entry_refs);

  init_freemap ();
  initialized = true;
  return true;
}

uint64_t
free_frames_count (void)
{
  return counter;
}
