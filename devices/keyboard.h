#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include <stdbool.h>

enum
{
  KBD_PRESS, // make
  KBD_RELEASE, // break
};

enum
{
  KBD_UNKNOWN = 0,

  KBD_ESC, KBD_CARET, KBD_TAB, KBD_CAPS, KBD_LEFT_SHIFT, KBD_RIGHT_SHIFT,
  KBD_CTLR, KBD_ALT, KBD_LESS, KBD_SPACE, KBD_BACK, KBD_ENTER, KBD_SCROLL,

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
  KBD_NUM_PUNCT
};

bool keyboard_init (void);

#endif
