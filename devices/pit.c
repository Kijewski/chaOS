#include "pit.h"

enum
{
  PIT_INTERRUPT_ONTERMINAL_COUNT = 0,
  PIT_ONE_SHOT                   = 1,
  PIT_RATE_GENERATOR             = 2,
  PIT_SQUARE_WAVE                = 3,
  PIT_SOFWARE_TRIGGERED_STROBE   = 4,
  PIT_HARDWARE_TRIGGERED_STROBE  = 5,
};

enum
{
  PIT_1 = 0x40,
  PIT_2 = 0x48,
};

enum
{
  PIT_REGISTER_0       = 0,
  PIT_REGISTER_1       = 1,
  PIT_REGISTER_2       = 2,
  PIT_REGISTER_CONTROL = 3,
};
