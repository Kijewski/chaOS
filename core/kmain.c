#include "kmain.h"
#include "interrupts.h"
#include "frame_allocator.h"
#include "paging.h"
#include "e820.h"
#include "quotes.h"
#include "keypress_handler.h"

#include <common/attributes.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <kernel.h>
#include <common/crX.h>
#include <devices/pic.h>
#include <devices/keyboard.h>
#include <devices/mouse.h>
#include <devices/videoram.h>
#include <common/random.h>
#include <common/cpuid.h>
#include <common/timeout.h>
#include <common/glue.h>

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
  videoram_printf ("Initializing %s: ", desc);

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
  videoram_puts ("Memory map (e820):\n", COLOR_NORMAL);
  for (const struct e820_ref *ref = e820_start (); ref; ref = e820_next (ref))
    {
      videoram_printf ("  * 0x%016" PRIxPTR " to 0x%016" PRIxPTR " is ",
                       ref->entry.base_addr,
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
          videoram_printf (" UNKNOWN (%d) ", ref->entry.type);
          break;
        }
      videoram_put_ln ();
    }
  videoram_puts ("Totalling up ", COLOR_NORMAL);
  videoram_put_int (total_memory/(1024*1024), COLOR_NORMAL);
  videoram_puts (" MB.\n\n", COLOR_NORMAL);
}

static void
put_welcoming_message (void)
{
  uint64_t r = random_get () % chaos_quotes_count;
  videoram_printf ("\n``%s''\n --  %s\n\n",
                   chaos_quotes[r][0], chaos_quotes[r][1]);

  videoram_puts ("Press ctrl+shift+esc+H to see possible commands.\n\n",
                 COLOR_NORMAL);
}

static void
put_cpu_info (void)
{
  videoram_printf ("CPU: %.12s (%.72s)\n",
                   cpuid_basic_info ().vendor_string,
                   cpuid_processor_brand_string ().string);
  uint64_t cpu_feats = cpuid_features ();
# define FEAT_EDX(NAME) \
    if (cpu_feats & CPUID_FEAT_EDX_##NAME) \
      videoram_puts (#NAME " ", COLOR_NORMAL);
# define FEAT_ECX(NAME) \
    if (cpu_feats & CPUID_FEAT_ECX_##NAME) \
      videoram_puts (#NAME " ", COLOR_NORMAL)

  videoram_puts ("Features: ", COLOR_NORMAL);
  FEAT_EDX (FPU); FEAT_EDX (VME); FEAT_EDX (DE); FEAT_EDX (PSE);
  FEAT_EDX (TSC); FEAT_EDX (MSR); FEAT_EDX (PAE); FEAT_EDX (MCE);
  FEAT_EDX (CX8); FEAT_EDX (APIC); FEAT_EDX (SEP); FEAT_EDX (MTRR);
  FEAT_EDX (PGE); FEAT_EDX (MCA); FEAT_EDX (CMOV); FEAT_EDX (PAT);
  FEAT_EDX (PSE36); FEAT_EDX (PSN); FEAT_EDX (CLF); FEAT_EDX (DTES);
  FEAT_EDX (ACPI); FEAT_EDX (MMX); FEAT_EDX (FXSR); FEAT_EDX (SSE);
  FEAT_EDX (SSE2); FEAT_EDX (SS); FEAT_EDX (HTT); FEAT_EDX (TM1);
  FEAT_EDX (IA64); FEAT_EDX (PBE);
  videoram_put_ln ();

  videoram_puts ("More features: ", COLOR_NORMAL);
  FEAT_ECX (SSE3); FEAT_ECX (PCLMUL); FEAT_ECX (DTES64); FEAT_ECX (MONITOR);
  FEAT_ECX (DS_CPL); FEAT_ECX (VMX); FEAT_ECX (SMX); FEAT_ECX (EST);
  FEAT_ECX (TM2); FEAT_ECX (SSSE3); FEAT_ECX (CID); FEAT_ECX (FMA);
  FEAT_ECX (CX16); FEAT_ECX (ETPRD); FEAT_ECX (PDCM); FEAT_ECX (DCA);
  FEAT_ECX (SSE4_1); FEAT_ECX (SSE4_2); FEAT_ECX (x2APIC); FEAT_ECX (MOVBE);
  FEAT_ECX (POPCNT); FEAT_ECX (AES); FEAT_ECX (XSAVE); FEAT_ECX (OSXSAVE);
  FEAT_ECX (AVX);
  videoram_put_ln ();
  
  videoram_put_ln ();
}

static bool
nx_bit_present (void)
{
  return true; // TODO
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

  // clear BSS:
  memset (&_section_bss_start[0], 0,
          &_section_bss_end[0] - &_section_bss_start[0]);

  videoram_cls (COLOR_NORMAL);

  // some welcoming information:
  videoram_printf ("\n  Welcome to \e%c chaOS! \n\n", COLOR_ERROR);
  put_cpu_info ();
  put_memory_map ();

  if (!nx_bit_present ())
    {
      videoram_puts (" Your CPU does not support the NX bit! \n", COLOR_ERROR);
      khalt ();
    }

  init_subsystem ("interrupt handling", &interrupts_init, NULL);

  videoram_puts ("Running a syscall test: ", COLOR_NORMAL);
  if (syscall_test ())
    videoram_puts (" ok \n", COLOR_INFO);
  else
    {
      videoram_puts (" FAIL \n", COLOR_ERROR);
      khalt ();
    }

  init_subsystem ("a syscall test", &syscall_test, NULL);
  init_subsystem ("PIC", &pic_init, NULL);
  pic_mask (~PIC_MASK_PIT); // TODO: setup PIT

  videoram_puts ("Setting CPU standards: ", COLOR_NORMAL);
  cr0_set_reset (CR0_WP|CR0_NE, CR0_MP|CR0_EM|CR0_NE|CR0_AM|CR0_CD|CR0_NW);
  msr_set_reset (MSR_EFER, EFER_NXE, 0);
  videoram_puts (" ok \n", COLOR_INFO);

  init_subsystem ("paging", &paging_init, NULL);

  init_subsystem ("timeout handler", &timeout_init, NULL);

  videoram_puts ("Enabling interrupts: ", COLOR_NORMAL);
  asm volatile ("sti");
  videoram_puts (" ok \n", COLOR_INFO);

  init_subsystem ("random number generator", &random_init, NULL);
  init_subsystem ("frame allocator", &frame_allocator_init, NULL);

  init_subsystem ("PS/2 keyboard", &keyboard_init, NULL);
  init_subsystem ("PS/2 mouse", &mouse_init, NULL);

  init_subsystem ("keypress handler", &keypress_handler_init, NULL);
  ENSURE (keypress_handler_set_keymap (KEYMAP_QWERTZ_DE_DE));

  // TODO: initialize more subsystems

  videoram_printf ("Available frames: %zu\n", free_frames_count ());

  put_welcoming_message ();

  for (;;)
    {
      int c = keypress_handler_getc ();
      if (!c)
        break;
      videoram_printf ("C: %c\n", c);
    }

  // TODO: do something
  for (;;)
    asm volatile ("hlt");
  
  khalt ();
}
