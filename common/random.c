#include "random.h"
#include <spinlock.h>
#include <devices/rtc.h>

static uint64_t seed[4];
static spinlock lock;

bool
random_init (void)
{
  spinlock_init (&lock);

  // TODO: proper seeds
  seed[0] = 0x0123456780234567ull;
  seed[1] = 0xDEADBEEFDEADBEEFull;
  seed[2] = 0xBAD1337C0FFEEBADull;

  struct rtc_data time;
  if (!rtc_read (&time))
    return false;

  seed[3] = ((uint64_t) time.second <<  0) +
            ((uint64_t) time.minute <<  8) +
            ((uint64_t) time.hour   << 16) +
            ((uint64_t) time.day    << 24) +
            ((uint64_t) time.month  << 32) +
            ((uint64_t) time.year   << 40);

  return true;
}

static inline uint64_t
rol (uint64_t v)
{
  asm volatile ("rolq $7, %0" : "+r"(v));
  return v;
}

uint64_t
random_get (void)
{
  spinlock_acquire (&lock);

  uint64_t result = seed[0] + seed[1] + seed[2] + seed[3];

  seed[0] ^= rol (seed[1]);
  seed[1] ^= rol (seed[2]);
  seed[2] ^= rol (seed[3]);
  seed[3] ^= rol (result);

  spinlock_release (&lock);
  return result;
}