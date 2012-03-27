#ifndef KBC_H__
#define KBC_H__

#include <stdbool.h>
#include <stdint.h>

enum
{
  KBD_STATUS_READABLE         = 1 << 0, // Most references name these bits in
  KBD_STATUS_NOT_WRITEABLE    = 1 << 1, //     the POV of the kbd controller!

  KBD_STATUS_BIST_OK          = 1 << 2,
  KBD_STATUS_LATEST_OP_WAS_64 = 1 << 3,
  KBD_STATUS_UNLOCKED         = 1 << 4,
                             // 1 << 5,
  KBD_STATUS_TIMEOUT          = 1 << 6,
  KBD_STATUS_PARITY_ERROR     = 1 << 7,
};

bool kbc_write_inbuf (uint8_t data);
bool kbc_read_outbuf (uint8_t *data);
bool kbc_send_cmd (uint8_t cmd);
bool kbc_clear_input (void);

static inline bool
kbc_clear_and_write_inbuf (uint8_t data)
{
  return kbc_clear_input () && kbc_write_inbuf (data);
}

#endif
