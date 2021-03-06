#include "shutdown.h"
#include "interrupts.h"
#include <common/ports.h>
#include <devices/kbc.h>

void NO_RETURN
system_shutdown (void)
{
  asm volatile ("cli");
  for (;;)
    {
      // (phony) ACPI shutdown (http://forum.osdev.org/viewtopic.php?t=16990)
      // Works for qemu and bochs.
      outw (0xB004, 0x2000);

      // Magic shutdown code for bochs and qemu.
      for (const char *s = "Shutdown"; *s; ++s)
        outb (0x8900, *s);

      // Magic code for VMWare. Also a hard lock.
      asm volatile ("cli; hlt");
    }
}

void NO_RETURN
system_reboot (void)
{
  interrupts_finit ();
  for (;;)
    {
      for (unsigned x = 0x10; x > 0; --x)
        {
          // try port 92h
          outb (0x92, inb (0x92) | 1);

          // Pulse CPU reset line to reboot.
          kbc_clear_input ();
          kbc_send_cmd (0xD1);
          while (!kbd_can_write ())
            continue;
          kbc_write_inbuf (0xFE);
        }

      // Pulsing the CPU reset line did not work.
      // Cause a tripple fault.
      asm volatile ("int3");
    }
}
