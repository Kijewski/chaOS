#include "assert.h"

#include <stdint.h>

void
assert_failure (const char *expr, const char *file, int line)
{
  // TODO
  (void) expr;
  (void) file;
  (void) line;

  asm volatile ("int $0x3");
}
