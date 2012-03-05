#include <stdint.h>
#include <string.h>
#include <devices/videoram.h>

void
_start (uint64_t lba_start, uint64_t lba_count, uint8_t lba_disk)
{
  videoram_cls (VR_COLOR (VR_GREEN|VR_BRIGHT, VR_BLACK));

  videoram_puts ("\n Welcome to ", VR_COLOR (VR_GREEN|VR_BRIGHT,
                                             VR_BLACK|VR_BRIGHT));
  videoram_puts (" chaOS! \n\n", VR_COLOR(VR_BLACK|VR_BRIGHT,
                                          VR_RED|VR_BRIGHT));

  for (;;)
    asm volatile ("hlt");
}
