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

#define SWAP(A,B)                                                             \
    ({                                                                        \
      typeof (A) *_a = &(A);                                                  \
      typeof (B) *_b = &(B);                                                  \
      typeof (*_a) _t = *_b;                                                  \
      *_b = *_a;                                                              \
      *_a = _t;                                                               \
      (void) 0;                                                               \
    })

#define ARRAY_LEN(X) (sizeof (X) / sizeof ((X)[0]))



#define _IN(KEY, ...)                                                         \
({                                                                            \
  __extension__ typedef typeof (KEY) _t;                                      \
  __extension__ register const _t _key = (KEY);                               \
  __extension__ const _t _values[] = { __VA_ARGS__ };                         \
  __extension__ register _Bool _r = 0;                                        \
  __extension__ register unsigned int _i;                                     \
  for (_i = 0; _i < ARRAY_LEN (_values); ++_i)                                \
    if (_key == _values[_i])                                                  \
      {                                                                       \
        _r = 1;                                                               \
        break;                                                                \
      }                                                                       \
  _r;                                                                         \
})

#endif
