#include "syscalls.h"

uint64_t
syscall_handle (unsigned num, void *pivot, va_list args)
{
  (void) num;
  (void) pivot;
  (void) args;
  return num;
}
