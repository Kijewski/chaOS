#include "keyboard.h"
#include "kbc.h"
#include "pic.h"

#include <common/attributes.h>
#include <common/intr.h>
#include <devices/videoram.h>
#include <core/shutdown.h>
#include <core/kmain.h>
#include <assert.h>

static const uint8_t KEYMAP_NORMAL[0x70] =
{
// 0x00
  0, KBD_ESC, KBD_1, KBD_2,
// 0x04
  KBD_3, KBD_4, KBD_5, KBD_6,
// 0x08
  KBD_7, KBD_8, KBD_9, KBD_0,
// 0x0C
  KBD_PUNCT_1_1, KBD_PUNCT_1_2, KBD_BACK, KBD_TAB,
// 0x10
  KBD_Q, KBD_W, KBD_E, KBD_R,
// 0x14
  KBD_T, KBD_Z, KBD_U, KBD_I,
// 0x18
  KBD_O, KBD_P, KBD_PUNCT_2_1, KBD_PUNCT_1_2,
// 0x1C
  KBD_ENTER, KBD_LEFT_CTLR, KBD_A, KBD_S,
// 0x20
  KBD_D, KBD_F, KBD_G, KBD_H,
// 0x24
  KBD_J, KBD_K, KBD_L, KBD_PUNCT_3_1,
// 0x28
  KBD_PUNCT_3_2, KBD_CARET, KBD_LEFT_SHIFT, KBD_PUNCT_2_3,
// 0x2C
  KBD_Y, KBD_X, KBD_C, KBD_V,
// 0x30
  KBD_B, KBD_N, KBD_M, KBD_PUNCT_4_1,
// 0x34
  KBD_PUNCT_4_2, KBD_PUNCT_4_3, KBD_RIGHT_SHIFT, KBD_NUM_TIMES,
// 0x38
  KBD_LEFT_ALT, KBD_SPACE, KBD_CAPS, KBD_F1,
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
  KBD_F12, 0, 0, 0,
// 0x5C
  0, 0, 0, 0,
// 0x60
  0, 0, 0, 0,
// 0x64
  0, 0, 0, 0,
// 0x68
  0, 0, 0, 0,
// 0x6C
  0, 0, 0, 0,
};

static const uint8_t KEYMAP_E0[0x70] =
{
// 0x00
  0, 0, 0, 0,
// 0x04
  0, 0, 0, 0,
// 0x08
  0, 0, 0, 0,
// 0x0C
  0, 0, 0, 0,
// 0x10
  KBD_MM_PREV, 0, 0, 0,
// 0x14
  0, 0, 0, 0,
// 0x18
  0, KBD_MM_NEXT, 0, 0,
// 0x1C
  KBD_NUM_ENTER, KBD_RIGHT_CTLR, 0, 0,
// 0x20
  KBD_MM_MUTE, 0, KBD_MM_PLAY, 0,
// 0x24
  KBD_MM_STOP, 0, 0, 0,
// 0x28
  0, 0, _KBD_FAKE_LSHIFT, 0,
// 0x2C
  0, 0, KBD_MM_VOL_DOWN, 0,
// 0x30
  KBD_MM_VOL_UP, 0, KBD_WWW_HOME, 0,
// 0x34
  0, KBD_NUM_SLASH, _KBD_FAKE_RSHIFT, KBD_PRINT_SCREEN,
// 0x38
  KBD_RIGHT_ALT, 0, 0, 0,
// 0x3C
  0, 0, 0, 0,
// 0x40
  0, 0, 0, 0,
// 0x44
  0, 0, 0, KBD_HOME,
// 0x48
  KBD_ARROW_UP, KBD_PAGE_UP, 0, KBD_ARROW_LEFT,
// 0x4C
  0, KBD_ARROW_RIGHT, 0, KBD_END,
// 0x50
  KBD_ARROW_DOWN, KBD_PAGE_DOWN, KBD_INS, KBD_DEL,
// 0x54
  0, 0, 0, 0,
// 0x58
  0, 0, 0, KBD_LOGO_LEFT,
// 0x5C
  KBD_LOGO_RIGHT, KBD_MENU, KBD_ACPI_POWER, KBD_ACPI_SLEEP,
// 0x60
  0, 0, 0, KBD_ACPI_WAKE,
// 0x64
  0, KBD_WWW_SEARCH, KBD_WWW_FAV, KBD_WWW_REFRESH,
// 0x68
  KBD_WWW_STOP, KBD_WWW_FORWARD, KBD_WWW_BACK, 0,
// 0x6C
  0, KBD_MM_SELECT, 0, 0,
};

static keyboard_handler_t *keyboard_handler_cb;

static bool keyboard_meta[_KBD_META_MAX];

bool
keyboard_meta_state (unsigned meta)
{
  if (meta < _KBD_META_MAX)
    return keyboard_meta[meta];

  if ((meta&0x80) && (meta&~0x80) < _KBD_META_80_COUNT)
    return keyboard_meta[2*(meta&~0x80)] || keyboard_meta[2*(meta&~0x80) + 1];

  return false;
}

static uint64_t keyboard_stateful = 0b100;

bool
keyboard_stateful_state (unsigned stateful)
{
  if (stateful < 64)
    return keyboard_stateful & (1 << stateful);
  else
    return false;
}

static void
keyboard_handle (int code, bool key_released)
{
// meta keys:
  switch (code)
    {
#define META_KEY(KEY, META)                         \
        case KEY:                                   \
          if (keyboard_meta[META] == !key_released) \
            return;                                 \
          keyboard_meta[META] = !key_released;      \
          break

    META_KEY (KBD_LEFT_SHIFT,   KBD_META_LSHIFT);
    META_KEY (KBD_RIGHT_SHIFT,  KBD_META_RSHIFT);
    META_KEY (KBD_LEFT_CTLR,    KBD_META_LCTRL);
    META_KEY (KBD_RIGHT_CTLR,   KBD_META_RCTRL);
    META_KEY (KBD_LEFT_ALT,     KBD_META_LALT);
    META_KEY (KBD_RIGHT_ALT,    KBD_META_RALT);
    META_KEY (KBD_LOGO_LEFT,    KBD_META_LLOGO);
    META_KEY (KBD_LOGO_RIGHT,   KBD_META_RLOGO);

    META_KEY (KBD_ESC,          KBD_META_ESCAPE);
    META_KEY (KBD_PRINT_SCREEN, KBD_META_PRINT);
    META_KEY (KBD_SCROLL,       KBD_META_SCROLL);
    META_KEY (KBD_PAUSE,        KBD_META_PAUSE);

    case _KBD_FAKE_LSHIFT:
    case _KBD_FAKE_RSHIFT:
      return;
    }

// stateful:

  switch (code)
    {
#define STATEFUL_KEY(KEY, STATEFUL)             \
        case KEY:                               \
          if (key_released)                     \
            keyboard_stateful ^= 1 << STATEFUL; \
          return

    STATEFUL_KEY (KBD_CAPS,   KBD_STATEFUL_CAPS);
    STATEFUL_KEY (KBD_SCROLL, KBD_STATEFUL_SCROLL);
    STATEFUL_KEY (KBD_NUM,    KBD_STATEFUL_NUM);
    }

// magic keycodes:
  if (keyboard_meta_state (KBD_META_CTRL) &&
      keyboard_meta_state (KBD_META_SHIFT) &&
      keyboard_meta_state (KBD_META_ESCAPE))
    {
      if (key_released)
        {
          switch (code)
            {
            case KBD_H:
              videoram_printf ("%s.\n",
                               "ctrl shift escape"
                               ": (H)elp"
                               ", (I)nterrupt test"
                               ", (L)ock+hang"
                               ", (R)eboot"
                               ", (Q)uit");
              return;
            case KBD_I:
              asm volatile ("int $0xCC");
              return;
            case KBD_L:
              khalt ();
              return;
            case KBD_Q:
              system_shutdown ();
              return;
            case KBD_R:
              system_reboot ();
              return;
            }
        }
      return;
    }

  if (keyboard_handler_cb != NULL)
    keyboard_handler_cb (code, key_released);
}

static void
keyboard_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  uint8_t datum;
  if (!kbd_can_read () || !kbc_read_outbuf (&datum))
    return;

  if ((datum & 0xe0) != 0xe0)
    {
      if (KEYMAP_NORMAL[datum & ~0x80])
        keyboard_handle (KEYMAP_NORMAL[datum & ~0x80], datum & 0x80);
      return;
    }

  if (datum == 0xe0)
    {
      if (!kbd_can_read () || !kbc_read_outbuf (&datum) ||
          (datum & ~0x80) >= 0x70)
        return;

      if (KEYMAP_E0[datum & ~0x80])
        {
          keyboard_handle (KEYMAP_E0[datum & ~0x80], datum & 0x80);
          return;
        }

      // videoram_printf ("Keyboard (e0): %02hhx\n", datum);
      return;
    }

  if (datum == 0xe1)
    {
      uint8_t datum1, datum2;
      if (!kbd_can_read () || !kbc_read_outbuf (&datum1) ||
          (datum1 & ~0x80) >= 0x70)
        return;
      if (!kbd_can_read () || !kbc_read_outbuf (&datum2) ||
          (datum2 & ~0x80) >= 0x70)
        return;

      if (datum1 == 0x1d && datum2 == 0x45)
        {
          keyboard_handle (KBD_PAUSE, false);
          return;
        }
      else if (datum1 == 0x9d && datum2 == 0xc5)
        {
          keyboard_handle (KBD_PAUSE, true);
          return;
        }

      // videoram_printf ("Keyboard (e1): %02hhx %02hhx\n", datum1, datum2);
      return;
    }

  kbc_clear_input ();
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

bool
keyboard_set_handler (keyboard_handler_t handler)
{
  keyboard_handler_cb = handler;
  return true;
}
