#include "kmain.h"

#include <attributes.h>
#include <stdlib.h>
#include <crX.h>
#include <devices/videoram.h>

#include "interrupts.h"
#include "paging.h"
#include "e820.h"

#define BG_NORMAL (VR_GREEN|VR_BRIGHT)
#define BG_INFO (VR_BLACK|VR_BRIGHT)
#define BG_ERROR BG_INFO
#define FG_NORMAL BG_INFO
#define FG_INFO BG_NORMAL
#define FG_ERROR (VR_RED|VR_BRIGHT)

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
_start (void)
{
  cr0_set_reset (CR0_WP|CR0_NE, CR0_MP|CR0_EM|CR0_NE|CR0_AM|CR0_CD|CR0_NW);

  videoram_cls (COLOR_NORMAL);

  videoram_puts ("\n  Welcome to ", COLOR_NORMAL);
  videoram_puts (" chaOS! \n\n", COLOR_ERROR);

  uint64_t total_memory = 0;
  videoram_puts ("Memory map:\n", COLOR_NORMAL);
  for (const struct e820_ref *ref = E820_BASE; ref; ref = e820_next (ref))
    {
      videoram_puts ("  * ", COLOR_NORMAL);
      videoram_puts ("0x", COLOR_NORMAL);
      videoram_put_hex (ref->entry.base_addr, COLOR_NORMAL);
      videoram_puts (" to 0x", COLOR_NORMAL);
      videoram_put_hex (ref->entry.base_addr + ref->entry.length -1,
                        COLOR_NORMAL);
      videoram_puts (" is ", COLOR_NORMAL);
      switch (ref->entry.type)
        {
        case E820_MEMORY:
          videoram_puts (" usable ", COLOR_INFO);
          if (ref->entry.base_addr >= 1024*1024)
            total_memory += ref->entry.length;
          else
            videoram_puts (" (space cowardly ignored)", COLOR_NORMAL);
          break;
        case E820_RESERVED:
          videoram_puts ("reserved", COLOR_NORMAL);
          break;
        case E820_ACPI_RECLAIMABLE:
          videoram_puts ("ACPI (reclaimable, won't be used)", COLOR_NORMAL);
          break;
        case E820_ACPI_NVS:
          videoram_puts ("ACPI (non-volatile)", COLOR_NORMAL);
          break;
        case E820_BAD:
          videoram_puts (" defect! ", COLOR_ERROR);
          break;
        default:
          videoram_puts (" UNKNOWN ", COLOR_ERROR);
          break;
        }
      videoram_put_ln ();
    }
  videoram_puts ("Totalling up ", COLOR_NORMAL);
  videoram_put_int (total_memory/(1024*1024), COLOR_NORMAL);
  videoram_puts (" MB.\n\n", COLOR_NORMAL);

  init_subsystem ("paging", &paging_init, NULL);
  init_subsystem ("interrupt handling", &interrupts_init, NULL);
  // TODO: more subsystems
  asm volatile ("sti");

  // TODO: start the system

  halt ();
}
