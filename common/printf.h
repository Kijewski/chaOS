#ifndef __COMMON_PRINTF_H
#define __COMMON_PRINTF_H

#include "attributes.h"
#include <stdarg.h>
#include <stddef.h>

void __vprintf (const char *format, va_list args,
                void (*output) (char, void *), void *aux);

#endif
