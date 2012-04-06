#include "keypress_handler.h"
#include <devices/keyboard.h>
#include <common/intr.h>
#include <common/round.h>

#pragma GCC diagnostic ignored "-Wmultichar"

static int ringbuffer[32];
static unsigned ringbuffer_head, ringbuffer_length;

static const int keymap_de[_KEYMAP_COUNT][2][2][_KBD_CODE_COUNT] =
{
  [KEYMAP_QWERTZ_DE_DE] =
  #include "keypress_handler/qwertz_de_de.h"
  ,
  [KEYMAP_QWERTY_EN_US] =
  #include "keypress_handler/qwerty_en_us.h"
  ,
  [KEYMAP_AZERTY_FR_FR] =
  #include "keypress_handler/azerty_fr_fr.h"
  ,
};

static unsigned keypress_handler_keymap = _KEYMAP_INVALID;

static void
keypress_handler (unsigned code, bool released)
{
  if (released ||
      keypress_handler_keymap == _KEYMAP_INVALID ||
      keyboard_meta_state (KBD_META_LALT) ||
      keyboard_meta_state (KBD_META_CTRL))
    return;
  
  unsigned shift = keyboard_meta_state (KBD_META_SHIFT) ? 1 : 0;
  unsigned alt   = keyboard_meta_state (KBD_META_RALT)  ? 1 : 0;

  int c = keymap_de[keypress_handler_keymap][alt][shift][code];
  if (c == 0)
    return;

  if (ringbuffer_length >= ARRAY_LEN (ringbuffer))
    {
      // discard
      // TODO: make an annoying beep :)
      return;
    }

  ringbuffer[(ringbuffer_length+ringbuffer_head) % ARRAY_LEN (ringbuffer)] = c;
  ++ringbuffer_length;
}

bool
keypress_handler_init (void)
{
  return keyboard_set_handler (&keypress_handler);
}

int
keypress_handler_getc (void)
{
  bool old_intr_level;
  for (;;)
    {
      old_intr_level = intr_get_and_set_off ();
      if (ringbuffer_length > 0)
        break;
      intr_set (old_intr_level);

      __sync_synchronize ();
      asm volatile ("hlt"); // TODO: won't work w/ multiple CPUs
    }

  int result = ringbuffer[ringbuffer_head];

  --ringbuffer_length;
  ringbuffer_head = (ringbuffer_head+1) % ARRAY_LEN (ringbuffer);

  intr_set (old_intr_level);
  return result;
}

bool
keypress_handler_set_keymap (unsigned keymap)
{
  if (keymap >= _KEYMAP_COUNT)
    return false;
  keypress_handler_keymap = keymap;
  return true;
}
