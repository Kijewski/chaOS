#ifndef STDDEF_H__
#define STDDEF_H__

#include <attributes.h>
#include <stdint.h>

#define NULL ((void *) 0)

typedef intptr_t ptrdiff_t;
typedef int16_t wchar_t;
typedef typeof (sizeof (0)) size_t;

#define offsetof(TYPE, MEMBER) (__builtin_offsetof (TYPE, MEMBER))

#endif
