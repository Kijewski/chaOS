#include "random.h"
#include <spinlock.h>
#include <round.h>
#include <devices/rtc.h>

static uint64_t seed[4];
static spinlock lock;

static inline uint64_t
rol (uint64_t v)
{
  asm ("rolq $7, %0" : "+r"(v));
  return v;
}

static inline uint64_t
rdtsc (void)
{
  uint32_t v[2];
  asm ("rdtsc" : "=a"(v[0]), "=d"(v[1]));
  return v[0] ^ ((uint64_t) v[1] << 32);
}

static uint64_t
random_get_real (void)
{
  uint64_t result = seed[0] + seed[1] + seed[2] + seed[3];

  for (unsigned i = 0; i < ARRAY_LEN (seed) - 1; ++i)
    seed[i] ^= rol (seed[i+1]);
  seed[ARRAY_LEN (seed) - 1] ^= rol (result);

  return result;
}

uint64_t
random_get (void)
{
  spinlock_acquire (&lock);
  uint64_t result = random_get_real ();
  spinlock_release (&lock);
  return result ^ rdtsc ();
}

bool
random_init (void)
{
  spinlock_init (&lock);

  seed[0] = 0x0123456780234567ull;
  seed[1] = 0xDEADBEEFDEADBEEFull;
  seed[2] = 0xBAD1337C0FFEEBADull;

  struct rtc_data time;
  if (!rtc_read (&time))
    return false;

  seed[ARRAY_LEN (seed) - 1] = ((uint64_t) time.second <<  0) +
                               ((uint64_t) time.minute <<  8) +
                               ((uint64_t) time.hour   << 16) +
                               ((uint64_t) time.day    << 24) +
                               ((uint64_t) time.month  << 32) +
                               ((uint64_t) time.year   << 40);

  for (unsigned i = 0; i < 2*ARRAY_LEN (seed); ++i)
    (void) random_get_real (); // mix the numbers

  return true;
}
