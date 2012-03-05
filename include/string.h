#ifndef STRING_H__
#define STRING_H__

#include <stdlib.h>
#include <attributes.h>

INLINE_ONLY (void *)
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

char    *strdup(const char *);
char    *strerror(int);
int     *strerror_r(int, char *, size_t);
char    *strncat(char *restrict, const char *restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char *restrict, const char *restrict, size_t);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
char    *strtok_r(char *, const char *, char **);
size_t   strxfrm(char *restrict, const char *restrict, size_t);

#endif
