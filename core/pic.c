#include "pic.h"
#include <ports.h>

#define PIC_MASTER_COMMAND (0x20)
#define PIC_MASTER_DATA    (0x21)
#define PIC_MASTER_IMR     (0x21)
#define PIC_SLAVE_COMMAND  (0xA0)
#define PIC_SLAVE_DATA     (0xA1)
#define PIC_SLAVE_IMR      (0xA1)

static void
pic_remap (void)
{
  // tell both PICs we want to initialize them
  outb (PIC_MASTER_COMMAND, 0x11);
  outb (PIC_SLAVE_COMMAND, 0x11);

  // which  ISR should be called if an IRQ occures
  outb (PIC_MASTER_DATA, PIC_MASTER_INTERRUPT_BASE);
  outb (PIC_SLAVE_DATA, PIC_SLAVE_INTERRUPT_BASE);

  // IRQ of the master the slave talks to
  outb (PIC_MASTER_DATA, 1 << PIC_NUM_SLAVE);
  outb (PIC_SLAVE_DATA, PIC_NUM_SLAVE);

  // use 8086 mode (nobody knows why ...), and automatic EOI
  outb (PIC_MASTER_DATA, 0x03);
  outb (PIC_SLAVE_DATA, 0x03);
}

void
pic_mask (int mask)
{
  outb (PIC_MASTER_IMR, (uint8_t) mask);
  outb (PIC_SLAVE_IMR, (uint8_t) mask >> 8);
}

bool
pic_init (void)
{
  pic_remap ();
  return true;
}
