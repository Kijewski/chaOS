#ifndef VIDEORAM_H__
#define VIDEORAM_H__

#include <stdint.h>
#include <common/attributes.h>
#include <common/itoa.h>
#include <stdarg.h>
#include <kernel.h>

#define VR_BASE ((char *) 0xb8000 + VIRT_BASE)
#define VR_COLS (90)
#define VR_ROWS (60)

enum
{
  VR_BLACK,
  VR_BLUE,
  VR_GREEN,
  VR_CYAN,
  VR_RED,
  VR_PURPLE,
  VR_BROWN,
  VR_GRAY,
  VR_BRIGHT,
};

#define VR_COLOR(B,F) ((F) | ((B) << 4))

void videoram_putc (char c, char attributes);
void videoram_puts (const char *s, char attributes);
void videoram_cls (char attributes);

static inline void
videoram_put_int (int64_t v, char attributes)
{
  char s[I64_TO_DEC_BUF_SIZE];
  videoram_puts (i64_to_dec (v, s), attributes);
}

static inline void
videoram_put_hex (int64_t v, char attributes)
{
  char s[I64_TO_HEX_BUF_SIZE];
  videoram_puts (i64_to_hex (v, s), attributes);
}

static inline void
videoram_put_ln (void)
{
  videoram_putc ('\n', 0);
}

void videoram_put_all_hex (uint64_t v, char attributes);

void videoram_printf (const char *format, ...) PRINTF (1);
void videoram_vprintf (const char *format, va_list args);

unsigned videoram_pos_x (void);
unsigned videoram_pos_y (void);
void videoram_put_right (const char *s, char attributes);

#endif
