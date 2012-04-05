// A copy of http://lowlevel.eu/w/index.php?title=PS/2-Maus_Tutorial&oldid=9393
// by http://lowlevel.eu/wiki/Benutzer:Bjork

#include "mouse.h"
#include "kbc.h"
#include "pic.h"

#include <common/attributes.h>
#include <common/intr.h>
#include <devices/videoram.h>
#include <common/round.h>

enum
{
  ACK    = 0xFA,
  RESEND = 0xFE,
};

enum
{
  MOUSE_CMD_SCALING_1_1 = 0xE6,
  MOUSE_CMD_SCALING_2_1 = 0xE7,
  MOUSE_CMD_RESOLUTION  = 0xE8,
  MOUSE_CMD_GET_STATUS  = 0xE9,
  MOUSE_CMD_STREAM_MODE = 0xEA,
  MOUSE_CMD_GET_DATUM   = 0xEB,
  MOUSE_CMD_ACK_MODE    = 0xEC,
  MOUSE_CMD_WRAP_MODE   = 0xEE,
  MOUSE_CMD_REMOTE_MODE = 0xF0,
  MOUSE_CMD_GET_ID      = 0xF2,
  MOUSE_CMD_SAMPLE_RATE = 0xF3,
  MOUSE_CMD_ACTIVATE    = 0xF4,
  MOUSE_CMD_DEACTIVATE  = 0xF5,
  MOUSE_CMD_DEFAULTS    = 0xF6,
  MOUSE_CMD_COME_AGAIN  = 0xFE,
  MOUSE_CMD_RESET       = 0xFF,
};

enum
{
  MOUSE_STATUS_RIGHT_PRESSED = 1 << 0,
  MOUSE_STATUS_LEFT_PRESSED  = 1 << 2,
  MOUSE_STATUS_SCALING_2_1   = 1 << 4,
  MOUSE_STATUS_ACTIVE        = 1 << 5,
  MOUSE_STATUS_STREAM_MODE   = 1 << 6,
};

enum
{
  MOUSE_RESOLUTION_1MM = 0x00,
  MOUSE_RESOLUTION_2MM = 0x01,
  MOUSE_RESOLUTION_4MM = 0x02,
  MOUSE_RESOLUTION_8MM = 0x03,
};

enum
{
  MOUSE_SAMPLE_RATE_10  = 10,
  MOUSE_SAMPLE_RATE_20  = 20,
  MOUSE_SAMPLE_RATE_40  = 40,
  MOUSE_SAMPLE_RATE_60  = 60,
  MOUSE_SAMPLE_RATE_80  = 80,
  MOUSE_SAMPLE_RATE_100 = 100,
  MOUSE_SAMPLE_RATE_200 = 200,
};

enum
{
  MOUSE_MODE_NORMAL    = 0x00,
  MOUSE_MODE_WHEEL     = 0x03,
  MOUSE_MODE_5_BUTTONS = 0x04,

  _MOUSE_MODE_INVALID = -1,
};

static unsigned mouse_mode  = _MOUSE_MODE_INVALID;

static inline bool
mouse_send_cmd (uint8_t cmd)
{
  uint8_t cb;
  for (unsigned i = 0x100; i > 0; --i)
    {
      if (!kbc_send_cmd (0xd4) ||
          !kbc_clear_and_write_inbuf (cmd) ||
          !kbc_read_outbuf (&cb))
        return false;
      if (cb == ACK)
        return true;
      else if (cb == RESEND)
        continue;
      else
        break;
    }
  return true;
}

static inline bool
mouse_write_inbuf (uint8_t datum)
{
  return kbc_send_cmd (0x60) && kbc_clear_and_write_inbuf (datum);
}

static bool
mouse_set_sample_rate (uint8_t rate)
{
  return mouse_send_cmd (MOUSE_CMD_SAMPLE_RATE) && mouse_send_cmd (rate);
}

static bool
mouse_set_type (uint8_t rate1, uint8_t rate2, uint8_t rate3)
{
  return mouse_set_sample_rate (rate1) &&
         mouse_set_sample_rate (rate2) &&
         mouse_set_sample_rate (rate3);
}

struct mouse_packet_normal
{
  uint8_t status;
  int8_t x, y;
} PACKED;

struct mouse_packet_wheel
{
  uint8_t status;
  int8_t x, y, z;
} PACKED;

struct mouse_packet_5_buttons
{
  uint8_t status;
  int8_t x, y, z:4;
  uint8_t mouse_4:1, mouse_5:1, :2;
} PACKED;

static void
mouse_handler (int num UNUSED, struct interrupt_frame *f UNUSED)
{
  union {
    struct mouse_packet_normal    normal;
    struct mouse_packet_wheel     wheel;
    struct mouse_packet_5_buttons five;
    uint8_t data[4];
  } packet;
  
  switch (mouse_mode)
    {
    case MOUSE_MODE_NORMAL:
      if (!kbc_read_outbuf (&packet.data[0]) ||
          !kbc_read_outbuf (&packet.data[1]) ||
          !kbc_read_outbuf (&packet.data[2]))
        {
          kbc_clear_input ();
          return;
        }
      break;

    case MOUSE_MODE_WHEEL:
    case MOUSE_MODE_5_BUTTONS:
      if (!kbc_read_outbuf (&packet.data[0]) ||
          !kbc_read_outbuf (&packet.data[1]) ||
          !kbc_read_outbuf (&packet.data[2]) ||
          !kbc_read_outbuf (&packet.data[3]))
        {
          kbc_clear_input ();
          return;
        }
      break;

    default:
      kbc_clear_input (); // cannot happen
      return;
    }

  videoram_printf ("Mouse: % 3u %+ 3d %+ 3d % 3u\n",
                   packet.data[0],
                   (int64_t) (int8_t) packet.data[1],
                   (int64_t) (int8_t) packet.data[2],
                   packet.data[3]);
  // TODO: inform somebody about the mouse move
}

static bool
mouse_activate (void)
{
  // Enable the auxiliary mouse device

  if (!kbc_send_cmd (0xA8))
    return false;

  // Enable the interrupts

  if (!kbc_send_cmd (0x20))
    return false;
  uint8_t cb;
  if (!kbc_read_outbuf (&cb) || ! mouse_write_inbuf (cb | 2))
    return false;

  // use defaults (stream mode, 4mm resolution, 1:1 scaling, 100 Hz samples)

  if (!mouse_send_cmd (MOUSE_CMD_DEFAULTS))
    return false;

  // activate stream mode

  if (!mouse_send_cmd (MOUSE_CMD_ACTIVATE))
    return false;

  // set mouse mode

  if (!mouse_set_type (200, 200, 80) || // try wheel + 5 buttons
      !mouse_send_cmd (MOUSE_CMD_GET_ID) || !kbc_read_outbuf (&cb))
    return false;
  if (cb != MOUSE_MODE_5_BUTTONS)
    {
      if (!mouse_set_type (200, 100, 80) || // try wheel mouse
          !mouse_send_cmd (MOUSE_CMD_GET_ID) || !kbc_read_outbuf (&cb))
        return false;
    }
  if (!_IN (cb, MOUSE_MODE_NORMAL, MOUSE_MODE_WHEEL, MOUSE_MODE_5_BUTTONS))
    return false;
  mouse_mode = cb;

  if(!mouse_set_sample_rate (MOUSE_SAMPLE_RATE_100))
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
