// A copy of http://lowlevel.eu/w/index.php?title=PS/2-Maus_Tutorial&oldid=9393
// by http://lowlevel.eu/wiki/Benutzer:Bjork

#include "kbc.h"
#include <ports.h>
#include <nop.h>

static bool
clear_write (void)
{
  int64_t cnt = 0x1000;
  while (inb (0x64) & KBD_STATUS_NOT_WRITEABLE)
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
  int64_t cnt = 0x1000;
  while (!(inb (0x64) & KBD_STATUS_READABLE))
    {
      if (--cnt < 0)
        return false;
      for (int i = 0x1000; i > 0; --i)
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
      if (!(datum & KBD_STATUS_BIST_OK))
        return false;
      else if (!(datum & KBD_STATUS_READABLE))
        return true;
      else
        inb (0x60);
    }
}
