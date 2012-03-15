#include "kmain.h"
#include "interrupts.h"
#include "paging.h"
#include "pic.h"
#include "e820.h"
#include <attributes.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <crX.h>

void NO_RETURN
khalt (void)
{
  videoram_puts ("\n Halting the system! ", COLOR_ERROR);

  interrupts_finit ();
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
      khalt ();
    }
}

static void
put_memory_map (void)
{
  uint64_t total_memory = 0;
  videoram_puts ("Memory map:\n", COLOR_NORMAL);
  for (const struct e820_ref *ref = E820_BASE; ref; ref = e820_next (ref))
    {
      videoram_puts ("  * ", COLOR_NORMAL);
      videoram_puts ("0x", COLOR_NORMAL);
      videoram_put_all_hex (ref->entry.base_addr, COLOR_NORMAL);
      videoram_puts (" to 0x", COLOR_NORMAL);
      videoram_put_all_hex (ref->entry.base_addr + ref->entry.length -1,
                            COLOR_NORMAL);
      videoram_puts (" is ", COLOR_NORMAL);
      switch (ref->entry.type)
        {
        case E820_MEMORY:
          videoram_puts (" usable ", COLOR_INFO);
          if (ref->entry.base_addr >= 1024*1024)
            total_memory += ref->entry.length;
          else
            videoram_puts (" (ignored)", COLOR_NORMAL);
          break;
        case E820_RESERVED:
          videoram_puts ("reserved", COLOR_NORMAL);
          break;
        case E820_ACPI_RECLAIMABLE:
          videoram_puts ("ACPI (reclaimable)", COLOR_NORMAL);
          break;
        case E820_ACPI_NVS:
          videoram_puts ("ACPI (non-volatile)", COLOR_NORMAL);
          break;
        case E820_BAD:
          videoram_puts (" defect! ", COLOR_ERROR);
          break;
        default:
          videoram_puts (" UNKNOWN (", COLOR_ERROR);
          videoram_put_int (ref->entry.type, COLOR_ERROR);
          videoram_puts (") ", COLOR_ERROR);
          break;
        }
      videoram_put_ln ();
    }
  videoram_puts ("Totalling up ", COLOR_NORMAL);
  videoram_put_int (total_memory/(1024*1024), COLOR_NORMAL);
  videoram_puts (" MB.\n\n", COLOR_NORMAL);
}

void
_start (void)
{
  // clear BSS
  memset (&_section_bss_start[0], 0,
          &_section_bss_end[0] - &_section_bss_start[0]);

  cr0_set_reset (CR0_WP|CR0_NE, CR0_MP|CR0_EM|CR0_NE|CR0_AM|CR0_CD|CR0_NW);
  msr_set_reset (MSR_EFER, EFER_NXE, 0);

  // debugging
  /*
  volatile char xxx = 0;
  while (xxx == 0)
    asm volatile ("pause" ::: "memory");
  //*/

  videoram_cls (COLOR_NORMAL);

  videoram_puts ("\n  Welcome to ", COLOR_NORMAL);
  videoram_puts (" chaOS! \n\n", COLOR_ERROR);

  put_memory_map ();

  init_subsystem ("PIC", &pic_init, NULL);
  init_subsystem ("interrupt handling", &interrupts_init, NULL);
  init_subsystem ("paging", &paging_init, NULL);

  videoram_puts ("Enable paging: ", COLOR_NORMAL);
  paging_enable ();
  videoram_puts (" ok \n", COLOR_INFO);

  videoram_puts ("Enable interrupts: ", COLOR_NORMAL);
  asm volatile ("sti");
  videoram_puts (" ok \n", COLOR_INFO);

  // TODO: initialize more subsystems

  // TODO: do something
  
  khalt ();
}
