#ifndef STRING_H__
#define STRING_H__

#include <stdlib.h>
#include <attributes.h>

// IMPLEMENTED BY GCC

static inline void *
memchr (const void *s, int c, size_t n)
{
  return __builtin_memchr (s, c, n);
}

static inline int
memcmp (const void *dest, const void *src, size_t size)
{
  return __builtin_memcmp (dest, src, size);
}

static inline void *
memmove (void *dest, const void *src, size_t size)
{
  return __builtin_memmove (dest, src, size);
}

static inline void *
memset (void *s, int c, size_t n)
{
  return __builtin_memset (s, c, n);
}

static inline char *
strcat (char *restrict dest, const char *restrict src)
{
  return __builtin_strcat (dest, src);
}

static inline char *
strchr (const char *s, int c)
{
  return __builtin_strchr (s, c);
}

static inline int
strcmp (const char *s1, const char *s2)
{
  return __builtin_strcmp (s1, s2);
}

int strcoll (const char *, const char *);

static inline char *
strcpy (char *restrict dest, const char *restrict src)
{
  return __builtin_strcpy (dest, src);
}

static inline size_t
strcspn (char *restrict s, const char *restrict reject)
{
  return __builtin_strcspn (s, reject);
}

static inline char *
strdup (const char *s)
{
  return __builtin_strdup (s);
}

static inline char *
strncat (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncat (dest, src, n);
}

static inline int
strncmp (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncmp (dest, src, n);
}

static inline char *
strncpy (char *restrict dest, const char *restrict src, size_t n)
{
  return __builtin_strncpy (dest, src, n);
}

static inline char *
strpbrk (const char *s, const char *accept)
{
  return __builtin_strpbrk (s, accept);
}

static inline char *
strrchr (const char *s, int c)
{
  return __builtin_strrchr (s, c);
}

static inline size_t
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

char *strstr (const char *, const char *) DEPRECATED;
char *strtok (char *restrict, const char *restrict) DEPRECATED;
size_t strxfrm (char *restrict, const char *restrict, size_t) DEPRECATED;

#endif
