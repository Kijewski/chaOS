#include "assert.h"
#include <stdint.h>
#include <devices/videoram.h>
#include <core/kmain.h>

void NO_RETURN
assert_failure (const char *expr, const char *file, int line)
{
  videoram_printf ("\e%c\n Failed assertion \"\e%c%s\e%c\" in file "
                   "\"\e%c%s\e%c\" at line \e%c%u\e%c. \n",
                   COLOR_ERROR, COLOR_INFO, expr,
                   COLOR_ERROR, COLOR_INFO, file,
                   COLOR_ERROR, COLOR_INFO, line,
                   COLOR_ERROR);

  asm volatile ("int3");
  UNREACHABLE ();
}
