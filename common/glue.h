#ifndef __COMMON_GLUE_H
#define __COMMON_GLUE_H

#include <stdint.h>
#include <stdbool.h>

enum
{
  SYSCALL_echo,
  SYSCALL_open,
  SYSCALL_close,
  SYSCALL_stat,
  SYSCALL_fstat,
  SYSCALL_read,
  SYSCALL_write,
  SYSCALL_mkdir,
  SYSCALL__exit,
  SYSCALL_isatty,
  SYSCALL_link,
  SYSCALL_lseek,
  SYSCALL_sbrk,
  SYSCALL_kill,
  SYSCALL_execve,
  SYSCALL_fork,
  SYSCALL_wait,
  SYSCALL_fcntl,
  SYSCALL_gettimeofday,
  SYSCALL_sigprocmask,
  SYSCALL_unlink,
  SYSCALL__init,
  SYSCALL__fini,
  SYSCALL_getpid,
  SYSCALL_times,
};

uint64_t syscall_call (void *pivot, ...);

bool syscall_test (void);

#endif
