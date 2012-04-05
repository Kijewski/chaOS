#ifndef __COMMON_PRINTF_H
#define __COMMON_PRINTF_H

#include "attributes.h"
#include <stdarg.h>
#include <stddef.h>

/* Standard functions. */
int snprintf (char *, size_t, const char *, ...) PRINTF_FORMAT (3, 4);
int vsnprintf (char *, size_t, const char *, va_list) PRINTF_FORMAT (3, 0);

/* Internal functions. */
void __vprintf (const char *format, va_list args,
                void (*output) (char, void *), void *aux);

#endif
