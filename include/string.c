#define NO_STRING_H_BUILTINS
#include "string.h"

#include <stdint.h>
#include <stddef.h>
#include <attributes.h>

void *
memcpy (void *dest, const void *src, size_t size)
{
  char *d = dest;
  const char *s = src;
  while (size-- > 0)
    *d++ = *s++;
  return dest;
}

int
memcmp (const void *dest, const void *src, size_t size)
{
  const char *d = dest;
  const char *s = src;
  while (size-- > 0)
    if (*d != *s)
      return *d < *s ? -1 : +1;
  return 0;
}

void *
memmove (void *dest, const void *src, size_t size)
{
  if ((uint64_t) dest < (uint64_t) src)
    return memcpy (dest, src, size);
  else if ((uint64_t) dest > (uint64_t) src)
    {
      char *d = dest + size;
      const char *s = src + size;
      while (size-- > 0)
        *--d = *--s;
    }
  return dest;
}

void *
memset (void *s_, int c, size_t n)
{
  char *s = s_;
  while (n-- > 0)
    *s++ = (char) c;
  return s_;
}

void *
memccpy (void *restrict dest, const void *restrict src, int c, size_t n)
{
  uint8_t *d = dest;
  const uint8_t *s = src;
  uint8_t t = (uint8_t) c;
  while (n-- > 0)
    if ((*d++ = *s++) == t)
      return d;
  return NULL;
}
