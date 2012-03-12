#ifndef STRINGS_H__
#define STRINGS_H__

#include <stddef.h>
#include <attributes.h>

static inline int
bcmp (const void *s1, const void *s2, size_t n)
{
  return __builtin_bcmp (s1, s2, n);
}


static inline void
bcopy (const void *src, void *dest, size_t n)
{
  __builtin_bcopy (src, dest, n);
}

static inline void
bzero (void *s, size_t n)
{
  __builtin_bzero (s, n);
}

static inline int
ffs (int i)
{
  return __builtin_ffs (i);
}

static inline char *
index (const char *s, int c)
{
  return __builtin_index (s, c);
}

static inline char *
rindex (const char *s, int c)
{
  return __builtin_rindex (s, c);
}

static inline int
strcasecmp (const char *s1, const char *s2)
{
  return __builtin_strcasecmp (s1, s2);
}

static inline int
strncasecmp (const char *s1, const char *s2, size_t n)
{
  return __builtin_strncasecmp (s1, s2, n);
}


#endif
