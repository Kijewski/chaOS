#ifndef __COMMON_GLUE_H
#define __COMMON_GLUE_H

#include <stdint.h>
#include <stdbool.h>

enum
{
  SYS_echo = 1,
  SYS_open,
  SYS_close,
  SYS_stat,
  SYS_fstat,
  SYS_read,
  SYS_write,
  SYS_mkdir,
  SYS__exit,
  SYS_isatty,
  SYS_link,
  SYS_lseek,
  SYS_sbrk,
  SYS_kill,
  SYS_execve,
  SYS_fork,
  SYS_wait,
  SYS_fcntl,
  SYS_gettimeofday,
  SYS_sigprocmask,
  SYS_unlink,
  SYS__init,
  SYS__fini,
  SYS_getpid,
  SYS_times,
};

bool syscall_test (void);
uint64_t syscall_call (void *pivot, ...);

#endif
