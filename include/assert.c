#include "assert.h"
#include <stdint.h>
#include <devices/videoram.h>
#include <core/kmain.h>

void
assert_failure (const char *expr, const char *file, int line)
{
  videoram_puts ("\n Failed assertion \"", COLOR_ERROR);
  videoram_puts (expr, COLOR_INFO);
  videoram_puts ("\" in file \"", COLOR_ERROR);
  videoram_puts (file, COLOR_INFO);
  videoram_puts ("\" at line ", COLOR_ERROR);
  videoram_put_int (line, COLOR_INFO);
  videoram_puts (". \n", COLOR_ERROR);

  asm volatile ("int $0x3");
}
