#include "paging.h"
#include "frame_allocator.h"

#include <common/attributes.h>
#include <common/round.h>
#include <kernel.h>
#include <assert.h>
#include <devices/videoram.h>

// MAX_KERNEL_SIZE_MB is so big b/c the ensure_toplevel_entry is not implemented
// very well, yet.
// Drawback: at least 4 MB of RAM are needed!
#define MAX_KERNEL_SIZE_MB (1024)

static uint64_t plm4[512] ALIGNED (0x1000);

#define TOPLEVEL_TYPE (PT_P | PT_RW | PT_US | PT_PWT)

static bool
ensure_toplevel_entry (uintptr_t *p)
{
  if (*p & PT_P)
    return true;
  void *f = frame_allocator_get ();
  paging_identity_map (f, PT_P|PT_RW|PT_PWT|PT_NX); // TODO: proper mapping!
  if (!f)
    return false;
  *p = (uint64_t) f | TOPLEVEL_TYPE;
  return true;
}

bool
paging_map (void *page, void *frame, uint64_t type)
{
  ASSERT (((uintptr_t) page & ((1<<12) - 1)) == 0);
  ASSERT (((uintptr_t) frame & ((1<<12) - 1)) == 0);

  // videoram_printf ("Page %8p -> frame %8p.\n", page, frame);

  uint64_t i_plm4 = ((uint64_t) page >> (12 + 9*3)) & ((1<<9) - 1);
  uintptr_t *i = &plm4[0];
  if (!ensure_toplevel_entry (&i[i_plm4]))
    return false;

  uint64_t i_pdp = ((uint64_t) page >> (12 + 9*2)) & ((1<<9) - 1);
  i = (void *) (i[i_plm4] & ~((1<<12) - 1));
  if (!ensure_toplevel_entry (&i[i_pdp]))
    return false;

  uint64_t i_pd = ((uint64_t) page >> (12 + 9*1)) & ((1<<9) - 1);
  i = (void *) (i[i_pdp] & ~((1<<12) - 1));
  if (!ensure_toplevel_entry (&i[i_pd]))
    return false;

  uint64_t i_pt = ((uint64_t) page >> (12 + 9*0)) & ((1<<9) - 1);
  i = (void *) (i[i_pd] & ~((1<<12) - 1));
  if (i[i_pt] & PT_P)
    return false;

  i[i_pt] = (uintptr_t) frame | type;
  paging_invalidate_pte (page);
  return true;
}

uint64_t *
paging_get_pte (void *page)
{
  ASSERT (((uintptr_t) page & ((1<<12) - 1)) == 0);

  uint64_t i_plm4 = ((uint64_t) page >> (12 + 9*3)) & ((1<<9) - 1);
  uintptr_t *i = &plm4[0];
  if (!(i[i_plm4] & PT_P))
    return NULL;

  uint64_t i_pdp = ((uint64_t) page >> (12 + 9*2)) & ((1<<9) - 1);
  i = (void *) (i[i_plm4] & ~((1<<12) - 1));
  if (!(i[i_pdp] & PT_P))
    return NULL;

  uint64_t i_pd = ((uint64_t) page >> (12 + 9*1)) & ((1<<9) - 1);
  i = (void *) (i[i_pdp] & ~((1<<12) - 1));
  if (!(i[i_pd] & PT_P))
    return NULL;

  uint64_t i_pt = ((uint64_t) page >> (12 + 9*0)) & ((1<<9) - 1);
  return &i[i_pt];
}

uint64_t
paging_unmap (void *page)
{
  uint64_t *pte = paging_get_pte (page);
  if (!pte)
    return PAGING_INV_RES;

  uint64_t result = *pte;
  if (!(result & PT_P))
    return result;
  *pte &= ~PT_P;
  paging_invalidate_pte (page);
  return *pte;
}

uint64_t
paging_remap (void *page)
{
  uint64_t *pte = paging_get_pte (page);
  if (!pte)
    return PAGING_INV_RES;

  uint64_t result = *pte;
  if (!result)
    return result;
  *pte |= PT_P;
  paging_invalidate_pte (page);
  return *pte;
}

bool
paging_init (void)
{
  static uint64_t pdp_0[512] ALIGNED (0x1000);
  static uint64_t pd_0[512] ALIGNED (0x1000);
  static uint64_t pt_0[(MAX_KERNEL_SIZE_MB+1) / 2][512] ALIGNED (0x1000);

  auto inline void
  mark_pt (void *p, uint64_t type)
  {
    unsigned i =  (uintptr_t) p / (2*1024*1024);
    unsigned j = ((uintptr_t) p % (2*1024*1024)) / 0x1000;
    pt_0[i][j] = (uintptr_t) p | type;
  }

  plm4[0]  = (uintptr_t) &pdp_0[0] | TOPLEVEL_TYPE;
  pdp_0[0] = (uintptr_t) &pd_0[0]  | TOPLEVEL_TYPE;
  for (unsigned i = 0; i < ARRAY_LEN (pt_0); ++i)
    pd_0[i]  = (uintptr_t) &pt_0[i][0] | TOPLEVEL_TYPE;

# define mark_section(SECTION, TYPE)                  \
    for (uint8_t *p = &_section_##SECTION##_start[0]; \
         p < &_section_##SECTION##_end[0];            \
         p += 0x1000)                                 \
      mark_pt (p, TYPE)

  mark_section (text,   PT_P|      PT_PWT);
  mark_section (rodata, PT_P|      PT_PWT|PT_NX);
  mark_section (data,   PT_P|PT_RW|PT_PWT|PT_NX);
  mark_section (ehframe,PT_P|PT_RW|PT_PWT|PT_NX);
  mark_section (bss,    PT_P|PT_RW|PT_PWT|PT_NX);

  for (uintptr_t i = 0; i < VR_COLS*VR_ROWS*2; i += 0x1000) // videoram
    mark_pt (VR_BASE + i, PT_P|PT_RW|PT_PWT|PT_NX);

  for (uintptr_t p = 0x4000; p < 0x8000; p += 0x1000) // stack and e820 data
    mark_pt ((void *) p, PT_P|PT_RW|PT_PWT|PT_NX);

  asm volatile ("mov %0, %%cr3" :: "a"(&plm4[0]) : "memory");
  return true;
}
