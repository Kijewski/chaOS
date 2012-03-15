#include "keyboard.h"
#include <core/pic.h>
#include <attributes.h>
#include <assert.h>
#include <ports.h>
#include <nop.h>

enum
{
  KBD_STATUS_READABLE         = 1 << 0, // Most references name these bit in
  KBD_STATUS_NOT_WRITEABLE    = 1 << 1, //    the POV of the kbd controller!

  KBD_STATUS_BIST_OK          = 1 << 2,
  KBD_STATUS_LATEST_OP_WAS_64 = 1 << 3,
  KBD_STATUS_UNLOCKED         = 1 << 4,
                             // 1 << 5,
  KBD_STATUS_TIMEOUT          = 1 << 6,
  KBD_STATUS_PARITY_ERROR     = 1 << 7,
};

static char ring_buffer[4096];
static unsigned ring_buffer_start;
static unsigned ring_buffer_count;

static void
keyboard_send_command (uint8_t cmd)
{
  while (inb (0x64) & KBD_STATUS_NOT_WRITEABLE)
    continue;
  outb (0x60, cmd);
}

static void
keyboard_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  while (inb (0x64) & KBD_STATUS_READABLE)
    {
      // TODO
      inb (0x60);
      (void) ring_buffer;
      (void) ring_buffer_count;
      (void) ring_buffer_start;
    }
}

bool
keyboard_init (void)
{
  pic_set_handler (PIC_NUM_KEYBOARD_CONTROLLER_1, &keyboard_handler);

  // Does the kbd controller think it is alright?
  ENSURE ((inb (0x64) & KBD_STATUS_BIST_OK) != 0);

  // clear input buffer
  while (inb (0x64) & KBD_STATUS_READABLE)
    inb (0x60);

  // activate keyboard
  keyboard_send_command (0xF4);

  return true;
}
