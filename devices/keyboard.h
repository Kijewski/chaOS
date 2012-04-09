#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include <stdbool.h>

enum
{
  _KBD_UNKNOWN = 0,

  KBD_ESC, KBD_CARET, KBD_TAB, KBD_CAPS, KBD_LEFT_SHIFT, KBD_RIGHT_SHIFT,
  KBD_LEFT_CTLR, KBD_LEFT_ALT, KBD_LESS, KBD_SPACE, KBD_BACK, KBD_ENTER,
  KBD_SCROLL,

  KBD_PUNCT_1_1, KBD_PUNCT_1_2,
  KBD_PUNCT_2_1, KBD_PUNCT_2_2, KBD_PUNCT_2_3,
  KBD_PUNCT_3_1, KBD_PUNCT_3_2,
  KBD_PUNCT_4_1, KBD_PUNCT_4_2, KBD_PUNCT_4_3,

  KBD_F1, KBD_F2, KBD_F3, KBD_F4, KBD_F5, KBD_F6, KBD_F7, KBD_F8, KBD_F9,
  KBD_F10, KBD_F11, KBD_F12,

  KBD_A, KBD_B, KBD_C, KBD_D, KBD_E, KBD_F, KBD_G, KBD_H, KBD_I, KBD_J, KBD_K,
  KBD_L, KBD_M, KBD_N, KBD_O, KBD_P, KBD_Q, KBD_R, KBD_S, KBD_T, KBD_U, KBD_V,
  KBD_W, KBD_X, KBD_Y, KBD_Z,

  KBD_0, KBD_1, KBD_2, KBD_3, KBD_4, KBD_5, KBD_6, KBD_7, KBD_8, KBD_9,

  KBD_NUM_0, KBD_NUM_1, KBD_NUM_2, KBD_NUM_3, KBD_NUM_4, KBD_NUM_5,
  KBD_NUM_6, KBD_NUM_7, KBD_NUM_8, KBD_NUM_9,
  KBD_NUM, KBD_NUM_TIMES, KBD_NUM_PLUS, KBD_NUM_MINUS,
  KBD_NUM_PUNCT,

// e0

  KBD_NUM_ENTER, KBD_RIGHT_CTLR, KBD_NUM_SLASH, KBD_PRINT_SCREEN, KBD_RIGHT_ALT,
  KBD_HOME, KBD_END, KBD_PAGE_UP, KBD_PAGE_DOWN,
  KBD_ARROW_UP, KBD_ARROW_DOWN, KBD_ARROW_LEFT, KBD_ARROW_RIGHT,
  KBD_INS, KBD_DEL,
  KBD_LOGO_LEFT, KBD_LOGO_RIGHT,
  KBD_MENU,

  KBD_ACPI_POWER, KBD_ACPI_SLEEP, KBD_ACPI_WAKE,

  KBD_MM_NEXT, KBD_MM_PREV, KBD_MM_STOP, KBD_MM_PLAY, KBD_MM_MUTE,
  KBD_MM_VOL_UP, KBD_MM_VOL_DOWN,
  KBD_MM_SELECT,
  
  KBD_WWW_SEARCH, KBD_WWW_HOME, KBD_WWW_BACK, KBD_WWW_FORWARD, KBD_WWW_STOP,
  KBD_WWW_REFRESH, KBD_WWW_FAV,

  _KBD_FAKE_LSHIFT, _KBD_FAKE_RSHIFT,

// e1

  KBD_PAUSE,

  _KBD_CODE_COUNT,
};

enum
{
  KBD_META_LSHIFT = 0x00, KBD_META_RSHIFT = 0x01, KBD_META_SHIFT = 0x80,
  KBD_META_LCTRL  = 0x02, KBD_META_RCTRL  = 0x03, KBD_META_CTRL  = 0x81,
  KBD_META_LALT   = 0x04, KBD_META_RALT   = 0x05, KBD_META_ALT   = 0x82,
  KBD_META_LLOGO  = 0x06, KBD_META_RLOGO  = 0x07, KBD_META_LOGO  = 0x83,

  KBD_META_ESCAPE = 0x08,
  KBD_META_PRINT  = 0x09,
  KBD_META_SCROLL = 0x0A,
  KBD_META_PAUSE  = 0x0B,

  _KBD_META_80_COUNT = 0x04,
  _KBD_META_MAX = 0x0C,
};

enum
{
  KBD_STATEFUL_CAPS,
  KBD_STATEFUL_SCROLL,
  KBD_STATEFUL_NUM,
};

bool keyboard_init (void);

bool keyboard_meta_state (unsigned meta);
bool keyboard_stateful_state (unsigned stateful);

typedef void keyboard_handler_t (unsigned code, bool released);
bool keyboard_set_handler (keyboard_handler_t handler);

#endif
