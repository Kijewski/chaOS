#ifndef STDARG_H__
#define STDARG_H__

#include <attributes.h>

typedef __builtin_va_list va_list;

#define va_start(V,N) (__builtin_va_start ((V), (N)))
#define va_arg(V,T)   (__builtin_va_arg ((V), typeof (T)))
#define va_copy(D,S)  (__builtin_va_copy ((D), (S)))
#define va_end(V)     (__builtin_va_end ((V)))

#endif
