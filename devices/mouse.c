// A copy of http://lowlevel.eu/w/index.php?title=PS/2-Maus_Tutorial&oldid=9393
// by http://lowlevel.eu/wiki/Benutzer:Bjork

#include "mouse.h"
#include "kbc.h"

#include <core/pic.h>
#include <attributes.h>
#include <intr.h>

enum
{
  ACK = 0xFA,
};

static void
mouse_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  kbc_clear_input (); // TODO
}

static inline bool
mouse_send_cmd (uint8_t cmd)
{
  return kbc_clear_input () &&
         kbc_send_cmd (0xd4) &&
         kbc_write_inbuf (cmd);
}

static bool
mouse_activate (void)
{
  //aktiviere die Maus beim Keyboard Controller
  if (!kbc_send_cmd (0xA8))
    return false;

  //Bit 1 im Command Byte setzen
  if (!kbc_send_cmd (0x20))
    return false;

  uint8_t cb;
  if (!kbc_read_outbuf (&cb))
    return false;
  if (!kbc_send_cmd (0x60))
    return false;
  if (!kbc_clear_and_write_inbuf (cb | 2))
    return false;

  //Standards setzen
  if (!mouse_send_cmd (0xF6))
    return false;
  if (!kbc_read_outbuf (&cb) || cb != ACK)
    return false;

  //das Senden von Daten aktivieren
  if (!mouse_send_cmd (0xF4))
    return false;
  if (!kbc_read_outbuf (&cb) || cb != ACK)
    return false;

  return true;
}

bool
mouse_init (void)
{
  pic_set_handler (PIC_NUM_KEYBOARD_CONTROLLER_2, &mouse_handler);

  bool old_level = intr_get_and_set_off ();
  bool result = mouse_activate ();
  intr_set (old_level);

  return result;
}
