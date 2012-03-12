#ifndef SPINLOCK_H__
#define SPINLOCK_H__

#include <stdint.h>
#include <attributes.h>
#include <assert.h>

typedef volatile struct
{
  uint8_t value;
} ALIGNED (0x10) spinlock;

static inline void
spinlock_acquire (spinlock *lock)
{
  while (__sync_lock_test_and_set (&lock->value, 1))
    continue;
}

static inline void
spinlock_release (spinlock *lock)
{
  uint8_t old_value UNUSED = __sync_lock_test_and_set (&lock->value, 0);
  ASSERT (old_value != 0);
}

#define SPINLOCK_INITIALIZER ((spinlock) { 0u })

#endif
