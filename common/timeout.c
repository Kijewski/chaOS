#include "timeout.h"

#include <nop.h>
#include <inttypes.h>
#include <round.h>
#include <assert.h>
#include <devices/rtc.h>
#include <devices/videoram.h>

enum
{
  TIMEOUT_MEASURE_SECONDS = 1,
  TIMEOUT_NOPS_BETWEEN_TESTS = 0x4000,
  TIMEOUT_DOT_PERIOD = 0x10,
};

GLOBAL_CASSERT (TIMEOUT_MEASURE_SECONDS > 0);

static uint64_t timeout_speed;

bool
timeout_init (void)
{
  uint8_t second[2];

  second[0] = rtc_read_second ();
  do
    second[1] = rtc_read_second ();
  while (second[0] == second[1]);

  uint64_t count = 0;
  for (int h = 0; h < TIMEOUT_MEASURE_SECONDS; ++h)
    {
      second[0] = second[1];
      do
        {
          for (uint64_t i = TIMEOUT_NOPS_BETWEEN_TESTS; i > 0; --i)
            {
              ++count;
              expensive_nop ();
            }

          if (count % (TIMEOUT_DOT_PERIOD *
                       TIMEOUT_NOPS_BETWEEN_TESTS *
                       TIMEOUT_MEASURE_SECONDS) == 0)
            videoram_printf (".");

          second[1] = rtc_read_second ();
        }
      while (second[0] == second[1]);
    }

  timeout_speed = count;
  videoram_printf (" (%"PRIu64".%03"PRIu64"M) ",
                   timeout_speed/TIMEOUT_MEASURE_SECONDS/1000/1000,
                   timeout_speed/TIMEOUT_MEASURE_SECONDS/1000%1000);

  return true;
}

void
timeout_s (uint64_t duration)
{
  for (uint64_t i = timeout_speed*duration; i > 0; --i)
    expensive_nop ();
}

void
timeout_ms (uint64_t duration)
{
  for (uint64_t i = duration*timeout_speed / TIMEOUT_MEASURE_SECONDS / 1000;
       i > 0; --i)
    expensive_nop ();
}

void
timeout_ns (uint64_t duration)
{
  for (uint64_t i = duration*timeout_speed / TIMEOUT_MEASURE_SECONDS / 1000000;
       i > 0; --i)
    expensive_nop ();
}
