#ifndef PIT_H__
#define PIT_H__

#include <stdint.h>
#include <stdbool.h>
#include <devices/pic.h>

enum PIT_OPERATING_MODE
{
  PIT_INTERRUPT_ONTERMINAL_COUNT = 0 << 1,
  PIT_ONE_SHOT                   = 1 << 1,
  PIT_RATE_GENERATOR             = 2 << 1,
  PIT_SQUARE_WAVE                = 3 << 1,
  PIT_SOFWARE_TRIGGERED_STROBE   = 4 << 1,
  PIT_HARDWARE_TRIGGERED_STROBE  = 5 << 1,
};

enum PIT_CHANNEL
{
  PIT_CHANNEL_0 = 0 << 6,
               // 1 << 6,
  PIT_CHANNEL_2 = 2 << 6,
};

bool pit_set (enum PIT_CHANNEL ch, enum PIT_OPERATING_MODE op, uint32_t hz);

static inline void
pit_set_handler (intr_handler_fun fun)
{
  pic_set_handler (PIC_NUM_PIT, fun);
}

static inline bool
pit_init_20hz (void)
{
  return pit_set (PIT_CHANNEL_0, PIT_RATE_GENERATOR, 20);
}

static inline bool
pit_init_33hz (void)
{
  return pit_set (PIT_CHANNEL_0, PIT_RATE_GENERATOR, 33);
}

static inline bool
pit_init_100hz (void)
{
  return pit_set (PIT_CHANNEL_0, PIT_RATE_GENERATOR, 100);
}

#endif
