#ifndef __CORE_KEYPRESS_HANDLER_H
#define __CORE_KEYPRESS_HANDLER_H

#include <stdbool.h>

enum
{
  KEYMAP_QWERTZ_DE_DE,
  KEYMAP_QWERTY_EN_US,
  KEYMAP_AZERTY_FR_FR,

  _KEYMAP_COUNT,
  _KEYMAP_INVALID,
};

bool keypress_handler_init (void);
bool keypress_handler_set_keymap (unsigned);

int keypress_handler_getc (void);

#endif
