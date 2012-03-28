#include "kmain.h"
#include "interrupts.h"
#include "frame_allocator.h"
#include "pic.h"
#include "paging.h"
#include "e820.h"
#include "kmalloc.h"

#include <attributes.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <kernel.h>
#include <crX.h>
#include <devices/keyboard.h>
#include <devices/mouse.h>
#include <nop.h>

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
  videoram_printf ("\e%cInitializing %s:", COLOR_NORMAL, desc);

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
  for (const struct e820_ref *ref = e820_start (); ref; ref = e820_next (ref))
    {
      videoram_printf ("\e%c  * 0x%016"PRIxPTR" to 0x%016"PRIxPTR" is ",
                       COLOR_NORMAL, ref->entry.base_addr,
                       ref->entry.base_addr + ref->entry.length - 1);

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
          videoram_printf ("\e%c UNKNOWN (%d) ", COLOR_ERROR, ref->entry.type);
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
  // debugging
  /*
  volatile char xxx = 0;
  while (xxx == 0)
    asm volatile ("pause" ::: "memory");
  //*/

  // clear BSS
  memset (&_section_bss_start[0], 0,
          &_section_bss_end[0] - &_section_bss_start[0]);

  cr0_set_reset (CR0_WP|CR0_NE, CR0_MP|CR0_EM|CR0_NE|CR0_AM|CR0_CD|CR0_NW);
  msr_set_reset (MSR_EFER, EFER_NXE, 0);

  videoram_cls (COLOR_NORMAL);
  expensive_nop ();

  videoram_printf ("\n\e%c  Welcome to \e%c chaOS! \n\n",
                   COLOR_NORMAL, COLOR_ERROR);

  put_memory_map ();

  init_subsystem ("interrupt handling", &interrupts_init, NULL);

  init_subsystem ("PIC", &pic_init, NULL);
  pic_mask (~PIC_MASK_PIT); // TODO: setup PIT

  init_subsystem ("paging", &paging_init, NULL);

  videoram_puts ("Enabling interrupts: ", COLOR_NORMAL);
  asm volatile ("sti");
  videoram_puts (" ok \n", COLOR_INFO);

  init_subsystem ("frame allocator", &frame_allocator_init, NULL);
  init_subsystem ("kernel memory allocator", &kmalloc_init, NULL);

  init_subsystem ("PS/2 keyboard", &keyboard_init, NULL);
  init_subsystem ("PS/2 mouse", &mouse_init, NULL);

  // TODO: initialize more subsystems

  videoram_printf ("\e%cAvailable frames: %zu\n", COLOR_NORMAL,
                   free_frames_count ());

  // TODO: do something
  for (;;)
    asm volatile ("hlt");
  
  khalt ();
}
