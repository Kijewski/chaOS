#ifndef STRING_H__
#define STRING_H__

#include <stdlib.h>
#include <attributes.h>

// IMPLEMENTED BY GCC

INLINE_ONLY (void *)
memchr (const void *s, int c, size_t n)
{
  return __builtin_memchr (s, c, n);
}

INLINE_ONLY (int)
memcmp (const void *dest, const void *src, size_t size)
{
  return __builtin_memcmp (dest, src, size);
}

INLINE_ONLY (void *)
memmove (void *dest, const void *src, size_t size)
{
  return __builtin_memmove (dest, src, size);
}

INLINE_ONLY (void *)
memset (void *s, int c, size_t n)
{
  return __builtin_memset (s, c, n);
}

INLINE_ONLY (char *)
strcat (char *restrict dest, const char *restrict src)
{
  return __builtin_strcat (dest, src);
}

INLINE_ONLY (char *)
strchr (const char *s, int c)
{
  return __builtin_strchr (s, c);
}

INLINE_ONLY (int)
strcmp (const char *s1, const char *s2)
{
  return __builtin_strcmp (s1, s2);
}

int strcoll (const char *, const char *);

INLINE_ONLY (char *)
strcpy (char *restrict dest, const char *restrict src)
{
  return __builtin_strcpy (dest, src);
}

INLINE_ONLY (size_t)
strcspn (char *restrict s, const char *restrict reject)
{
  return __builtin_strcspn (s, reject);
}

INLINE_ONLY (char *)
strdup (const char *s)
{
  return __builtin_strdup (s);
}

INLINE_ONLY (char *)
strncat (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncat (dest, src, n);
}

INLINE_ONLY (int)
strncmp (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncmp (dest, src, n);
}

INLINE_ONLY (char *)
strncpy (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncpy (dest, src, n);
}

INLINE_ONLY (char *)
strpbrk (const char *s, const char *accept)
{
  return __builtin_strpbrk (s, accept);
}

INLINE_ONLY (char *)
strrchr (const char *s, int c)
{
  return __builtin_strrchr (s, c);
}

INLINE_ONLY (size_t)
strspn (const char *s, const char *accept)
{
  return __builtin_strspn (s, accept);
}

// MANUALLY IMPLEMENTED

static inline void *
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

char *strerror (int); // TODO
char *strerror_r (int, char *, size_t); // TODO
char *strtok_r (char *s, const char *delim, char **save_ptr); // TODO

// UNIMPLEMENTED!
char *strstr(const char *, const char *) DEPRECATED;
char *strtok(char *restrict, const char *restrict) DEPRECATED;
size_t strxfrm (char *restrict, const char *restrict, size_t) DEPRECATED;

#endif
