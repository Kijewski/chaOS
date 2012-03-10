#ifndef ROUND_H__
#define ROUND_H__

#include <stdint.h>
#include <attributes.h>

static inline uint64_t
round_down_pow2 (uint64_t val, uint8_t exp)
{
  return val & ~((1 << exp) - 1);
}

static inline uint64_t
round_up_pow2 (uint64_t val, uint8_t exp)
{
  uint64_t mask = (1 << exp) - 1;
  if (val & ~mask)
    return (val | mask) + 1;
  else
    return val;
}

#define MIN(A,B)                                                              \
    ({                                                                        \
      typeof (A) _a = (A);                                                    \
      typeof (B) _b = (B);                                                    \
      _a <= _b ? _a : _b;                                                     \
    })
#define MAX(A,B)                                                              \
    ({                                                                        \
      typeof (A) _a = (A);                                                    \
      typeof (B) _b = (B);                                                    \
      _a >= _b ? _a : _b;                                                     \
    })


#endif
