#include "videoram.h"

#include <attributes.h>
#include <ports.h>

#define VR_BASE ((volatile char *) 0xb8000)
#define VR_COLS (80)
#define VR_ROWS (25)

static unsigned vr_xpos, vr_ypos;

static inline void
videoram_putc_at (char c, char attributes, unsigned x, unsigned y)
{
  VR_BASE[2*(x + y*VR_COLS) + 0] = c;
  VR_BASE[2*(x + y*VR_COLS) + 1] = attributes;
}

static void
update_cursor (void)
{
  unsigned short position = vr_ypos*VR_COLS + vr_xpos;

  // cursor LOW port to vga INDEX register
  outb (0x3D4, 0x0F);
  outb (0x3D5, (unsigned char) (position & 0xFF));
  // cursor HIGH port to vga INDEX register
  outb (0x3D4, 0x0E);
  outb (0x3D5, (unsigned char ) ((position >> 8) & 0xFF));
}

static void
videoram_putc (char c, char attributes)
{
  if (c != '\n')
    {
      videoram_putc_at (c, attributes, vr_xpos, vr_ypos);
      ++vr_xpos;
    }
  if (c == '\n' || vr_xpos >= VR_COLS)
    {
      vr_xpos = 0;
      ++vr_ypos;
      if (vr_ypos >= VR_ROWS)
        vr_ypos = 0;
    }
}

void
videoram_puts (const char *s, char attributes)
{
  while (*s)
    videoram_putc (*s++, attributes);
  update_cursor ();
}

void 
videoram_cls (char attributes)
{
  unsigned x, y;
  for (y = 0; y < VR_ROWS; ++y)
    for (x = 0; x < VR_COLS; ++x)
      videoram_putc_at (' ', attributes, x, y);
  vr_xpos = vr_ypos = 0;
  update_cursor ();
}
