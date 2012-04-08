// A copy of http://lowlevel.eu/w/index.php?title=PS/2-Maus_Tutorial&oldid=9393
// by http://lowlevel.eu/wiki/Benutzer:Bjork

#include "kbc.h"
#include <common/ports.h>
#include <common/nop.h>

bool
kbd_can_read (void)
{
  return inb (0x64) & KBD_STATUS_READABLE;
}

bool
kbd_can_write (void)
{
  return !(inb (0x64) & KBD_STATUS_NOT_WRITEABLE);
}

static bool
clear_write (void)
{
  int64_t cnt = 0x100;
  while (!kbd_can_write ())
    {
      if (--cnt < 0)
        return false;
      for (int i = 0x10; i > 0; --i)
        expensive_nop (); // TODO: proper sleeping
    }
  return true;
}

static bool
clear_read (void)
{
  int64_t cnt = 0x100;
  while (!kbd_can_read ())
    {
      if (--cnt < 0)
        return false;
      for (int i = 0x10; i > 0; --i)
        expensive_nop (); // TODO: proper sleeping
    }
  return true;
}

bool
kbc_write_inbuf (uint8_t data)
{
  if (clear_write ())
    outb (0x60, data);
  else
    return false;
  return true;
}

bool
kbc_read_outbuf (uint8_t *data)
{
  if (clear_read ())
    *data = inb (0x60);
  else
    return false;
  return true;
}

bool
kbc_send_cmd (uint8_t cmd)
{
  if (clear_write ())
    outb (0x64, cmd);
  else
    return false;
  return true;
}

bool
kbc_clear_input (void)
{
  for (;;)
    {
      uint8_t datum = inb (0x64);
/* bochs:
      if (!(datum & KBD_STATUS_BIST_OK))
        return false;
*/
      if (!(datum & KBD_STATUS_READABLE))
        return true;
      inb (0x60);
    }
}
