#ifndef SPINLOCK_H__
#define SPINLOCK_H__

#include <stdint.h>
#include <attributes.h>
#include <assert.h>

struct spinlock
{
  uint8_t value;
} ALIGNED (0x10);

INLINE_ONLY (void)
spinlock_acquire (struct spinlock *lock)
{
  while (__sync_lock_test_and_set (&lock->value, 1))
    continue;
}

INLINE_ONLY (void)
spinlock_release (struct spinlock *lock)
{
  uint8_t old_value UNUSED = __sync_lock_test_and_set (&lock->value, 0);
  ASSERT (old_value != 0);
}

#endif
