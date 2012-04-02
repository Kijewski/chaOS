#include "keyboard.h"
#include "kbc.h"

#include <core/pic.h>
#include <attributes.h>
#include <intr.h>
#include <devices/videoram.h>

static char ring_buffer[4096] UNUSED;
static unsigned ring_buffer_start UNUSED;
static unsigned ring_buffer_count UNUSED;

static const uint8_t KEYMAP_NORMAL[0x60] =
{
// 0x00
  0,KBD_ESC,KBD_1,KBD_2,
// 0x04
  KBD_3,KBD_4,KBD_5,KBD_6,
// 0x08
  KBD_7,KBD_8,KBD_9,KBD_0,
// 0x0C
  KBD_PUNCT_1_1,KBD_PUNCT_1_2,KBD_BACK,KBD_TAB,
// 0x10
  KBD_Q,KBD_W,KBD_E,KBD_R,
// 0x14
  KBD_T,KBD_Z,KBD_U,KBD_I,
// 0x18
  KBD_O,KBD_P,KBD_PUNCT_2_1,KBD_PUNCT_1_2,
// 0x1C
  KBD_ENTER,KBD_CTLR,KBD_A,KBD_S,
// 0x20
  KBD_D,KBD_F,KBD_G,KBD_H,
// 0x24
  KBD_J,KBD_K,KBD_L,KBD_PUNCT_3_1,
// 0x28
  KBD_PUNCT_3_2,KBD_CARET,KBD_LEFT_SHIFT,KBD_PUNCT_2_3,
// 0x2C
  KBD_Y,KBD_X,KBD_C,KBD_V,
// 0x30
  KBD_B,KBD_N,KBD_M,KBD_PUNCT_4_1,
// 0x34
  KBD_PUNCT_4_2,KBD_PUNCT_4_3,KBD_RIGHT_SHIFT,KBD_NUM_TIMES,
// 0x38
  KBD_ALT,KBD_SPACE,KBD_CAPS,KBD_F1,
// 0x3C
  KBD_F2, KBD_F3, KBD_F4, KBD_F5,
// 0x40
  KBD_F6, KBD_F7, KBD_F8, KBD_F9,
// 0x44
  KBD_F10, KBD_NUM, KBD_SCROLL, KBD_NUM_7,
// 0x48
  KBD_NUM_8, KBD_NUM_9, KBD_NUM_MINUS, KBD_NUM_4,
// 0x4C
  KBD_NUM_5, KBD_NUM_6, KBD_NUM_PLUS, KBD_NUM_1,
// 0x50
  KBD_NUM_2, KBD_NUM_3, KBD_NUM_0, KBD_NUM_PUNCT,
// 0x54
  0, 0, KBD_LESS, KBD_F11,
// 0x58
  KBD_F12,0,0,0,
// 0x5C
  0,0,0,0,
};

static bool
keyboard_handle (void)
{
  uint8_t datum;
  if (!kbd_can_read () || !kbc_read_outbuf (&datum) || datum == 0)
    return false;

  if ((datum & ~0x80) < 0x60)
    {
      videoram_printf ("Keyboard (%s): 0x%02hhx\n",
                       datum&0x80 ? "release" : "press",
                       KEYMAP_NORMAL[datum & ~0x80]);
      // TODO
      return true;
    }
  else if (datum == 0xe0)
    {
    while (kbd_can_read () && kbc_read_outbuf (&datum))
      videoram_printf ("Keyboard (e0): %02hhx\n", datum);
      kbc_clear_input (); // TODO
      return true;
    }
  else if (datum == 0xe1)
    {
    while (kbd_can_read () && kbc_read_outbuf (&datum))
      videoram_printf ("Keyboard (e1): %02hhx\n", datum);
      kbc_clear_input (); // TODO
      return true;
    }
  else
    {
      kbc_clear_input ();
      return false;
    }
}

static void
keyboard_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  while (keyboard_handle ())
    continue;
}

static bool
keyboard_activate (void)
{
  return kbc_clear_and_write_inbuf (0xF4);
}

bool
keyboard_init (void)
{
  pic_set_handler (PIC_NUM_KEYBOARD_CONTROLLER_1, &keyboard_handler);

  bool old_level = intr_get_and_set_off ();
  bool result = keyboard_activate ();
  intr_set (old_level);

  return result;
}
