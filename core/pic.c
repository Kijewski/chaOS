#include "pic.h"
#include <ports.h>
#include <attributes.h>
#include <assert.h>

// assumed so in the code
GLOBAL_CASSERT (PIC_SLAVE_INTERRUPT_BASE == PIC_MASTER_INTERRUPT_BASE+8)

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

  // which  ISR should be called if an IRQ occurs
  outb (PIC_MASTER_DATA, PIC_MASTER_INTERRUPT_BASE);
  outb (PIC_SLAVE_DATA, PIC_SLAVE_INTERRUPT_BASE);

  // IRQ of the master the slave talks to
  outb (PIC_MASTER_DATA, 1 << PIC_NUM_SLAVE);
  outb (PIC_SLAVE_DATA, PIC_NUM_SLAVE);

  // use 8086 mode and automatic EOI
  outb (PIC_MASTER_DATA, 0x03);
  outb (PIC_SLAVE_DATA, 0x03);
}

void
pic_mask (int mask)
{
  outb (PIC_MASTER_IMR, (uint8_t) mask);
  outb (PIC_SLAVE_IMR, (uint8_t) mask >> 8);
}

intr_handler_fun *funs[16];

static void
irq_handler (int num, struct interrupt_frame *f)
{
  num -= PIC_MASTER_INTERRUPT_BASE;
  if (funs[num])
    funs[num] (num, f);
}

static void
pic_assign_handlers (void)
{
  for (int i = 0; i < 16; ++i)
    interrupts_set_handler (i + PIC_MASTER_INTERRUPT_BASE, &irq_handler);
}

bool
pic_init (void)
{
  pic_remap ();
  pic_assign_handlers ();
  return true;
}
