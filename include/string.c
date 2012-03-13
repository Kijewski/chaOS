// MUST NOT INCLUDE string.h!!
#define STRING_H__

#include <stdint.h>
#include <stddef.h>
#include <attributes.h>

void  *memchr (const void *s, int c, size_t n);
int    memcmp (const void *dest, const void *src, size_t size);
void  *memcpy (void *dest, const void *src, size_t size);
void  *memmove (void *dest, const void *src, size_t size);
void  *memset (void *s, int c, size_t n);
char  *strcat (char *restrict dest, const char *restrict src);
char  *strchr (const char *s, int c);
int    strcmp (const char *s1, const char *s2);
int    strcoll (const char *, const char *);;
char  *strcpy (char *restrict dest, const char *restrict src);
size_t strcspn (char *restrict s, const char *restrict reject);
char  *strdup (const char *s);
char  *strncat (char *restrict dest, const char *restrict src, size_t n);
int    strncmp (char *restrict dest, const char *restrict src, size_t n);
char  *strncpy (char *restrict dest, const char *restrict src, size_t n);
char  *strpbrk (const char *s, const char *accept);
char  *strrchr (const char *s, int c);
size_t strspn (const char *s, const char *accept);
void  *memccpy (void *restrict dest, const void *restrict src, int c, size_t n);
char *strerror (int);
char *strerror_r (int, char *, size_t);
char *strtok_r (char *s, const char *delim, char **save_ptr);

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
    {
      char *d = dest;
      const char *s = src;
      while (size-- > 0)
        *d++ = *s++;
    }
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
