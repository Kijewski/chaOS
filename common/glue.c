#define _COMPILING_NEWLIB 1

#include "glue.h"
#include "attributes.h"
#include "random.h"
#include <core/interrupts.h>

#pragma GCC optimize "omit-frame-pointer", "Os"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"

#define INT(NAME)                                                             \
    uint64_t __SYSCALL_##NAME ();                                             \
    uint64_t                                                                  \
    __SYSCALL_##NAME ()                                                       \
    {                                                                         \
      uint64_t result;                                                        \
      asm ("int %2"                                                           \
           : "=a"(result)                                                     \
           : "a"(SYS_##NAME), "i"(INT_SYSCALL)                                \
           : "memory");                                                       \
      return result;                                                          \
    }

INT (echo)
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

bool
syscall_test (void)
{
  uint64_t value = random_get ();
  uint64_t result = __SYSCALL_echo (value);
  return result == value;
}
