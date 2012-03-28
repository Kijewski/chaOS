#include "videoram.h"

#include <attributes.h>
#include <string.h>
#include <ports.h>
#include <assert.h>
#include <printf.h>
#include <stdbool.h>

static unsigned vr_xpos, vr_ypos;
static char cls_color = VR_COLOR (VR_BLACK, VR_GRAY);

static inline void
videoram_putc_at (char c, char attributes, unsigned x, unsigned y)
{
  ASSERT (x < VR_COLS);
  ASSERT (y < VR_ROWS);
  VR_BASE[2*(x + y*VR_COLS) + 0] = c;
  VR_BASE[2*(x + y*VR_COLS) + 1] = attributes;
}

void
videoram_putc (char c, char attributes)
{
  if (c != '\n')
    {
      videoram_putc_at (c, attributes, vr_xpos, vr_ypos);
      ++vr_xpos;
    }
  if (c == '\n' || vr_xpos >= VR_COLS)
    {
      for (int x = vr_xpos; x < VR_COLS; ++x)
        videoram_putc_at (' ', cls_color, x, vr_ypos);
      vr_xpos = 0;
      ++vr_ypos;
      if (vr_ypos >= VR_ROWS)
        {
          // TODO: scroll
          vr_ypos = 0;
        }
    }
}

void
videoram_puts (const char *s, char attributes)
{
  while (*s)
    videoram_putc (*s++, attributes);
}

void 
videoram_cls (char attributes)
{
  cls_color = attributes;
  unsigned x, y;
  for (y = 0; y < VR_ROWS; ++y)
    for (x = 0; x < VR_COLS; ++x)
      videoram_putc_at (' ', attributes, x, y);
  vr_xpos = vr_ypos = 0;
}

void
videoram_put_all_hex (uint64_t v, char attributes)
{
  char s[I64_TO_HEX_BUF_SIZE];
  memset (&s[2], '0', sizeof (s)-3);
  u64_to_hex (v, s);
  videoram_puts (&s[2], attributes);
}

void
videoram_printf (const char *format, ...)
{
  va_list args;
  va_start (args, format);
  videoram_vprintf (format, args);
  va_end (args);
}

struct videoram_vprintf_aux
{
  char attributes;
  bool got_e;
};

static void
videoram_vprintf_helper (char ch, void *aux_)
{
  struct videoram_vprintf_aux *aux = aux_;
  if (!aux->got_e)
    {
      if (ch != '\e')
        videoram_putc (ch, aux->attributes);
      else
        aux->got_e = true;
    }
  else
    {
      aux->attributes = ch;
      aux->got_e = false;
    }
}

void
videoram_vprintf (const char *format, va_list args)
{
  struct videoram_vprintf_aux aux = {
    .attributes = 7,
    .got_e = false,
  };

  __vprintf (format, args, &videoram_vprintf_helper, &aux);
}
