#ifndef STRING_H__
#define STRING_H__

#include <stdlib.h>
#include <attributes.h>

// (maybe) IMPLEMENTED BY GCC

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
size_t strlen (const char *s);

#ifndef NO_STRING_H_BUILTINS
# define memchr(...) (__builtin_memchr (__VA_ARGS__))
# define memcmp(...) (__builtin_memcmp (__VA_ARGS__))
# define memcpy(...) (__builtin_memcpy (__VA_ARGS__))
# define memmove(...) (__builtin_memmove (__VA_ARGS__))
# define memset(...) (__builtin_memset (__VA_ARGS__))
# define strcat(...) (__builtin_strcat (__VA_ARGS__))
# define strchr(...) (__builtin_strchr (__VA_ARGS__))
# define strcmp(...) (__builtin_strcmp (__VA_ARGS__))
# define strcoll(...) (__builtin_strcoll (__VA_ARGS__))
# define strcpy(...) (__builtin_strcpy (__VA_ARGS__))
# define strcspn(...) (__builtin_strcspn (__VA_ARGS__))
# define strdup(...) (__builtin_strdup (__VA_ARGS__))
# define strncat(...) (__builtin_strncat (__VA_ARGS__))
# define strncmp(...) (__builtin_strncmp (__VA_ARGS__))
# define strncpy(...) (__builtin_strncpy (__VA_ARGS__))
# define strpbrk(...) (__builtin_strpbrk (__VA_ARGS__))
# define strrchr(...) (__builtin_strrchr (__VA_ARGS__))
# define strspn(...) (__builtin_strspn (__VA_ARGS__))
#endif

// MANUALLY IMPLEMENTED

void  *memccpy (void *restrict dest, const void *restrict src, int c, size_t n);
char *strerror (int);
char *strerror_r (int, char *, size_t);
char *strtok_r (char *s, const char *delim, char **save_ptr);
size_t strnlen (const char *s, size_t maxlen);

// UNIMPLEMENTED!

char *strstr (const char *, const char *) DEPRECATED;
char *strtok (char *restrict, const char *restrict) DEPRECATED;
size_t strxfrm (char *restrict, const char *restrict, size_t) DEPRECATED;

#endif
