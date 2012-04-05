#ifndef __COMMON_SYSCALLS_C
#define __COMMON_SYSCALLS_C

#include "interrupts.h"
#include <stdint.h>

uint64_t syscall_handle (unsigned num, struct interrupt_frame *f);

#endif
