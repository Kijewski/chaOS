#ifndef SIGNAL_H__
#define SIGNAL_H__

#include <stdint.h>
#include <attributes.h>
#include <sys/types.h>

#define SIG_DFL (TODO)
#define SIG_ERR (TODO)
#define SIG_IGN (TODO)

typedef struct {
  uint64_t value;
} ALIGNED (0x10) sig_atomic_t;

union sigval
{
  int    sival_int; //   Integer signal value. 
  void  *sival_ptr; //   Pointer signal value. 
};

typedef struct
{
  int                    sigev_notify;            // Notification type. 
  int                    sigev_signo;             // Signal number. 
  union sigval           sigev_value;             // Signal value. 
  void (*sigev_notify_function) (union sigval);   // Notification function. 
  pthread_attr_t        *sigev_notify_attributes; // Notification attributes. 
} sigset_t;

// TODO

#endif
