#include "pit.h"

#include <common/ports.h>
#include <common/intr.h>
#include <assert.h>

enum
{
  PIT_CHANNEL_0_PORT = 0x40,
  PIT_CHANNEL_1_PORT = 0x41,
  PIT_CHANNEL_2_PORT = 0x42,
  PIT_COMMAND_PORT   = 0x43,
};

enum
{
  PIT_ACCESS_MODE_LATCH_COUNT = 0 << 4,
  PIT_ACCESS_MODE_LO_BYTE     = 1 << 4,
  PIT_ACCESS_MODE_HI_BYTE     = 2 << 4,
  PIT_ACCESS_MODE_LO_HI       = 3 << 4,
};

bool
pit_set (enum PIT_CHANNEL ch, enum PIT_OPERATING_MODE op, uint32_t hz)
{
  if (19 > hz || hz > 1193181)
    return false;

  uint16_t freq = (uint32_t) 1193181 / hz;

  bool old_intr = intr_get_and_set_off ();

  outb (PIT_COMMAND_PORT, op|PIT_ACCESS_MODE_LO_HI|ch);
  switch (ch)
    {
    case PIT_CHANNEL_0:
      outb (PIT_CHANNEL_0_PORT, freq & 0xFF);
      outb (PIT_CHANNEL_0_PORT, freq >> 8);
      break;
    case PIT_CHANNEL_2:
      outb (PIT_CHANNEL_2_PORT, freq & 0xFF);
      outb (PIT_CHANNEL_2_PORT, freq >> 8);
      break;
    }

  if (old_intr)
    intr_set (true);
  return true;
}
