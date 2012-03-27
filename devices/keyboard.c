#include "keyboard.h"
#include "kbc.h"

#include <core/pic.h>
#include <attributes.h>
#include <ports.h>

static char ring_buffer[4096] UNUSED;
static unsigned ring_buffer_start UNUSED;
static unsigned ring_buffer_count UNUSED;

static void
keyboard_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  kbc_clear_input (); // TODO
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

  asm volatile ("cli");
  bool result = keyboard_activate ();
  asm volatile ("sti");

  return result;
}
