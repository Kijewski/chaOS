#include "syscalls.h"
#include <common/glue.h>

static uint64_t
syscall_echo (struct interrupt_frame *f)
{
  return (uint64_t) f->rdi;
}

static uint64_t
syscall_open (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_close (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_stat (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_fstat (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_read (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_write (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_mkdir (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall__exit (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_isatty (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_link (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_lseek (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_sbrk (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_kill (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_execve (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_fork (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_wait (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_fcntl (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_gettimeofday (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_sigprocmask (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_unlink (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall__init (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall__fini (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_getpid (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

static uint64_t
syscall_times (struct interrupt_frame *f)
{
  return 0; // TODO
  (void) f;
}

uint64_t
syscall_handle (unsigned num, struct interrupt_frame *f)
{
  switch (num)
    {
#define HANDLE(NAME)                                                          \
        case SYSCALL_##NAME:                                                  \
          return syscall_##NAME (f);
    HANDLE (echo)
    HANDLE (open)
    HANDLE (close)
    HANDLE (stat)
    HANDLE (fstat)
    HANDLE (read)
    HANDLE (write)
    HANDLE (mkdir)
    HANDLE (_exit)
    HANDLE (isatty)
    HANDLE (link)
    HANDLE (lseek)
    HANDLE (sbrk)
    HANDLE (kill)
    HANDLE (execve)
    HANDLE (fork)
    HANDLE (wait)
    HANDLE (fcntl)
    HANDLE (gettimeofday)
    HANDLE (sigprocmask)
    HANDLE (unlink)
    HANDLE (_init)
    HANDLE (_fini)
    HANDLE (getpid)
    HANDLE (times)
    default:
      asm volatile ("int3");
      return -1ull;
    }
}
