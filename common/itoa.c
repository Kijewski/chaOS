#include "itoa.h"

const char ITOA_ALPHABET[] = "ZYXWVUTSRQPONMLKJIHGFEDCBA987654321"
                             "0"
                             "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char *
_i64_to_base (int64_t v, char *c, const char *H, int base)
{
  *c = 0;
  if (v != 0)
    for (;;)
      {
        typeof (v) r = v % base;
        v /= base;
        *--c = H[r];
        if (v == 0)
          {
            if (r < 0)
              *--c = '-';
            break;
          }
      }
  else
    *--c = '0';
  return c;
}
