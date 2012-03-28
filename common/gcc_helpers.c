#include "gcc_helpers.h"
#include <stddef.h>

// Took from:
//   http://fossies.org/dox/qemu-1.0.1/____umodti3_8c_source.html
//   http://fossies.org/dox/qemu-1.0.1/____udivti3_8c_source.html
//   http://fossies.org/dox/qemu-1.0.1/____udivmodti4_8c_source.html

uint128_t __umodti3 (uint128_t num, uint128_t den)
{
  uint128_t v;
  (void) __udivmodti4 (num, den, &v);
  return v;
}

uint128_t __udivti3 (uint128_t num, uint128_t den)
{
  return __udivmodti4 (num, den, NULL);
}

uint128_t __udivmodti4 (uint128_t num, uint128_t den, uint128_t *rem_p)
{
  if (den == 0)
    asm volatile ("int $0");

  uint128_t quot = 0, qbit = 1;
  while ((int128_t) den >= 0)
    {
      den <<= 1;
      qbit <<= 1;
    }

  while (qbit)
    {
      if (den <= num)
        {
          num -= den;
          quot += qbit;
        }
      den >>= 1;
      qbit >>= 1;
    }

  if (rem_p)
    *rem_p = num;

  return quot;
}
