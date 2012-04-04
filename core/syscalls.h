#ifndef __COMMON_SYSCALLS_C
#define __COMMON_SYSCALLS_C

#include <stdarg.h>
#include <stdint.h>
#include <glue.h>

uint64_t syscall_handle (unsigned num, void *pivot, va_list args);

#endif
