#include "glue.h"
#include <attributes.h>

static uint64_t __attribute__ ((__optimize__ (3, "omit-frame-pointer")))
syscall_int (unsigned num, void *pivot, va_list args)
{
  (void) num;
  (void) pivot;
  (void) args;
  uint64_t result;
  asm ("int $67" : "=a"(result));
  return result;
}

uint64_t
syscall_call (void *pivot, ...)
{
  unsigned num;
  asm volatile ("" : "=a"(num));

  va_list args;
  va_start (args, pivot);
  uint64_t result = syscall_int (num, pivot, args);
  va_end (args);

  return result;
}

#define INT(NAME)                                                             \
    void NAME (void);                                                         \
    void __attribute__ ((__optimize__ (3, "omit-frame-pointer")))             \
    NAME (void)                                                               \
    {                                                                         \
      asm ("jmp *%1" :: "a"(SYSCALL_##NAME), "r"(syscall_call));              \
      UNREACHABLE ();                                                         \
    }

INT (open)
INT (close)
INT (stat)
INT (fstat)
INT (read)
INT (write)
INT (mkdir)
INT (_exit)
INT (isatty)
INT (link)
INT (lseek)
INT (sbrk)
INT (kill)
INT (execve)
INT (fork)
INT (wait)
INT (fcntl)
INT (gettimeofday)
INT (sigprocmask)
INT (unlink)
INT (_init)
INT (_fini)
INT (getpid)
INT (times)
