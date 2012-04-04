#define NO_STRING_H_BUILTINS
#include "string.h"

#include <stdint.h>
#include <stddef.h>
#include <attributes.h>
#include <core/kmalloc.h>

void *
memchr (const void *s_, int c_, size_t n)
{
  uint8_t const *s = s_, c = c_;
  while (n-- > 0)
    if (*s == c)
      return (void *) s;
    else
      ++s;
  return NULL;
}

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

size_t
strlen (const char *s)
{
  size_t result = 0;
  while (*s++)
    ++result;
  return result;
}

size_t
strnlen (const char *s, size_t maxlen)
{
  size_t result = 0;
  while (*s++ && ++result < maxlen)
    continue;
  return result;
}

char *
strcat (char *restrict dest, const char *restrict src)
{
  char *d = dest;
  while (*d++)
    continue;
  strcpy (d, src);
  return dest;
}

char *
strchr (const char *s_, int c_)
{
  for (const char *s = s_, c = c_; *s; ++s)
    if (*s == c)
      return (char *) s;
  return NULL;
}

int
strcmp (const char *s, const char *d)
{
  for (;; ++s, ++d)
    if (*s != *d)
      return *d < *s ? -1 : +1;
    else if (!*s)
      return 0;
}

int
strcoll (const char *s, const char *d)
{
  return strcmp (s, d);
}

char *
strcpy (char *restrict dest_, const char *restrict src)
{
  char *dest = dest_;
  while (*src)
    *dest++ = *src++;
  return dest_;
}

size_t
strcspn (char *restrict s, const char *restrict reject)
{
  size_t result = 0;
  for (; *s; ++result, ++s)
    for (const char *r = reject; *r; ++r)
      if (*s == *r)
        return result;
  return result;
}

char *
strdup (const char *s)
{
  char *d = kmalloc (strlen (s) + 1, true);
  if (!d)
    return NULL;
  return strcpy (d, s);
}

char *
strncat (char *restrict dest, const char *restrict src, size_t n)
{
  char *d = dest;
  while (*d++)
    continue;
  strncpy (d, src, n);
  return dest;
}

int
strncmp (char *restrict s, const char *restrict d, size_t n)
{
  for (; n-- > 0; ++s, ++d)
    if (*s != *d)
      return *d < *s ? -1 : +1;
    else if (!*s)
      return 0;
  return 0;
}

char *
strncpy (char *restrict dest_, const char *restrict src, size_t n)
{
  char *dest = dest_;
  while (n-- > 0 && *src)
    *dest++ = *src++;
  return dest_;
}

char *
strpbrk (const char *s, const char *accept)
{
  for (; *s; ++s)
    for (const char *a = accept; *a; ++a)
      if (*s == *a)
        return (char *) s;
  return NULL;
}

char *
strrchr (const char *s_, int c_)
{
  const char *r = NULL;
  for (const char *s = s_, c = c_; *s; ++s)
    if (*s == c)
      r = s;
  return (char *) r;
}

size_t
strspn (const char *s, const char *accept)
{
  size_t result = 0;
  while (*s)
    {
      bool accepted = false;
      for (const char *a = accept; *a; ++a)
        if (*s == *a)
          {
            accepted = true;
            break;
          }
      if (!accepted)
        break;
      ++result;
    }
  return result;
}
