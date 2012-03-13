#include "shutdown.h"
#include "interrupts.h"
#include <ports.h>

void
system_shutdown (void)
{
  interrupts_finit ();
  for (;;)
    {
      // ACPI shutdown (http://forum.osdev.org/viewtopic.php?t=16990)
      // Works for qemu and bochs.
      outw (0xB004, 0x2000);

      // Magic shutdown code for bochs and qemu.
      for (const char *s = "Shutdown"; *s; ++s)
        outb (0x8900, *s);

      // Magic code for VMWare. Also a hard lock.
      asm volatile ("cli; hlt");
    }
}

void
system_reboot (void)
{
  interrupts_finit ();
  for (;;)
    {
      // Pulse CPU reset line to reboot.
      // Seems not to work in qemu. (Pintos uses many timer_udelay ...)
      for (int i = 0xFFFF; i >= 0; --i)
        {
          for (int j = 0xFFFF; j >= 0; --j)
            {
              uint8_t temp = inb (0x64);
              if (temp & 2)
                inb (0x60);
              else if (!(temp & 1))
                break;
            }
          outb (0x64, 0xFE);
        }

      // Pulsing the CPU reset line did not work.
      // Cause a tripple fault.
      asm volatile ("int3");
    }
}
