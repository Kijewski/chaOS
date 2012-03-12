#include "videoram.h"

#include <attributes.h>
#include <string.h>
#include <ports.h>

#define VR_BASE ((char *) 0xb8000)
#define VR_COLS (80)
#define VR_ROWS (50)

static unsigned vr_xpos, vr_ypos;
static char cls_color = VR_COLOR (VR_BLACK, VR_GRAY);

static inline void
videoram_putc_at (char c, char attributes, unsigned x, unsigned y)
{
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
  memset (&s[2], '0', sizeof (s)-1);
  u64_to_hex (v, s);
  videoram_puts (&s[2], attributes);
}
