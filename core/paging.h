#ifndef PAGING_H_
#define PAGING_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

enum
{
  PT_P   = 1 << 0,
  PT_RW  = 1 << 1,
  PT_US  = 1 << 2,
  PT_PWT = 1 << 3,
  PT_PCD = 1 << 4,
  PT_A   = 1 << 5,
  PT_D   = 1 << 6,
  PT_PAT = 1 << 7,
  PT_G   = 1 << 8,
  PT_NX  = 1ull << 63,
};

bool paging_init (void);

bool paging_map (void *page, void *frame, uint64_t type);

enum
{
  PAGING_INV_RES   = -1ull,
  PAGING_TYPE_MAP  = PT_NX | ((1<<12) - 1),
  PAGING_FRAME_MAP = ~PAGING_TYPE_MAP,
};

uint64_t paging_unmap (void *page);
uint64_t paging_remap (void *page);

uint64_t *paging_get_pte (void *page);
void paging_invalidate_pte (uint64_t *pte);

static inline bool
paging_get (void *page, void **frame, uint64_t *type)
{
  uint64_t *pte = paging_get_pte (page);
  if (!pte)
    return false;
  if (frame)
    *frame = (void *) (*pte & PAGING_FRAME_MAP);
  if (type)
    *type = *pte & PAGING_TYPE_MAP;
  return true;
}

static inline uint64_t
paging_identity_map (void *p, uint64_t type)
{
  return paging_map (p, p, type);
}

#endif
