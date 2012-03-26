#ifndef SYS_TIME_H__
#define SYS_TIME_H__

#include <time.h>
#include <sys/select.h>
#include <attributes.h>

struct timeval
{
  time_t tv_sec;
  suseconds_t tv_usec;
};

struct itimerval
{
  struct timeval it_interval;
  struct timeval it_value;
};

enum
{
  ITIMER_REAL,
  ITIMER_VIRTUAL,
  ITIMER_PROF,
};

int   getitimer(int, struct itimerval *);
int   gettimeofday(struct timeval *restrict, void *restrict);
int   select(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,
          struct timeval *restrict);
int   setitimer(int, const struct itimerval *restrict,
          struct itimerval *restrict);
int   utimes(const char *, const struct timeval [2]) DEPRECATED;

#endif
