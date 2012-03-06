#include "kmain.h"

#include <attributes.h>
#include <stdlib.h>
#include <crX.h>
#include <devices/videoram.h>

#include "interrupts.h"
#include "paging.h"

#define BG_NORMAL (VR_GREEN|VR_BRIGHT)
#define BG_INFO (VR_BLACK|VR_BRIGHT)
#define BG_ERROR (VR_RED|VR_BRIGHT)
#define FG_NORMAL BG_INFO
#define FG_INFO BG_NORMAL
#define FG_ERROR VR_BLACK

#define COLOR_NORMAL (VR_COLOR (BG_NORMAL, FG_NORMAL))
#define COLOR_INFO (VR_COLOR (BG_INFO, FG_INFO))
#define COLOR_ERROR (VR_COLOR (BG_ERROR, FG_ERROR))

static void NO_RETURN
halt (void)
{
  videoram_puts ("\n Halting the system! ", COLOR_ERROR);
  asm volatile ("cli");
  for (;;)
    asm volatile ("hlt");
}

static void
init_subsystem (const char *desc, bool (*init) (void), bool (*cleanup) (void))
{
  videoram_puts ("Initializing ", COLOR_NORMAL);
  videoram_puts (desc, COLOR_NORMAL);
  videoram_puts (": ", COLOR_NORMAL);

  if (init ())
    videoram_puts (" ok \n", COLOR_INFO);
  else
    {
      videoram_puts (" FAIL \n", COLOR_ERROR);
      if (cleanup)
        cleanup ();
      halt ();
    }
}

void
_start (uint64_t lba_start UNUSED,
        uint64_t lba_count UNUSED,
        uint8_t  lba_disk UNUSED)
{
  // No need to switch the stack.
  // It is 0x7C00 down to 0x0500, i.e. almost 30kb!

  cr0_set_reset (CR0_WP|CR0_NE, CR0_MP|CR0_EM|CR0_NE|CR0_AM|CR0_CD|CR0_NW);

  videoram_cls (COLOR_NORMAL);

  videoram_puts ("\n  Welcome to ", COLOR_NORMAL);
  videoram_puts (" chaOS! \n\n", VR_COLOR (FG_NORMAL, BG_ERROR));

  init_subsystem ("paging", &paging_init, NULL);
  init_subsystem ("interrupt handling", &interrupts_init, NULL);
  // TODO: more subsystems
  asm volatile ("sti");

  // TODO: start the system

  halt ();
}
