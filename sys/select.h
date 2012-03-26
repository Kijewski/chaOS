#ifndef SYS_SELECT_H__
#define SYS_SELECT_H__

#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

struct fd_set;
typedef struct fd_set fd_set;

void FD_CLR(int fd, fd_set *fdset);
int FD_ISSET(int fd, fd_set *fdset);
void FD_SET(int fd, fd_set *fdset);
void FD_ZERO(fd_set *fdset);

#define FD_SETSIZE (TODO)

int  pselect(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,
         const struct timespec *restrict, const sigset_t *restrict);
int  select(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,
         struct timeval *restri

#endif
