#ifndef BITMAP_H__
#define BITMAP_H__

#include "attributes.h"
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <wordsize.h>

#define BITMAP_INVALID_VALUE (-1ull)

GLOBAL_CASSERT (__WORDSIZE == 64)

#define BITMAP_SIZE(N) (((N) + 63) / 64)

static inline bool
bitmap_get (uint64_t *map, size_t index)
{
  return map[index/64] & (1 << (index%64));
}

static inline void
bitmap_mark (uint64_t *map, size_t index)
{
  map[index/64] |= 1 << (index%64);
}

static inline void
bitmap_reset (uint64_t *map, size_t index)
{
  map[index/64] &= ~(1 << (index%64));
}

static inline bool
bitmap_bts (uint64_t *map, size_t index)
{
  register int64_t result = index % 64;
  asm volatile ("bts %0, %1" : "+m"(map[index/64]) : "r"(result));
  return result;
}

static inline bool
bitmap_btr (uint64_t *map, size_t index)
{
  register int64_t result = index % 64;
  asm volatile ("btr %0, %1" : "+m"(map[index/64]) : "r"(result));
  return result;
}

static inline size_t
bitmap64_reset_least (uint64_t *map)
{
  uint64_t result;
  asm volatile ("bsf %1, %0\n"
                "btr %0, %1": "=r"(result), "+r"(*map));
  return result;
}

static inline size_t
bitmap64_mark_least (uint64_t *map)
{
  uint64_t v = ~*map;
  uint64_t result = bitmap64_reset_least (&v);
  *map = ~v;
  return result;
}

static inline size_t
bitmap_mark_least (uint64_t *map, size_t length)
{
  while (length-- != 0)
    {
      uint64_t v = ~*map;
      if (v != 0)
        {
          size_t result = bitmap64_reset_least (&v);
          *map = ~v;
          return result;
        }
      ++map;
    }
  return BITMAP_INVALID_VALUE;
}

static inline size_t
bitmap_reset_least (uint64_t *map, size_t length)
{
  while (length-- != 0)
    {
      if (*map != 0)
        return bitmap64_reset_least (map);
      ++map;
    }
  return BITMAP_INVALID_VALUE;
}

#endif
