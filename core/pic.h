#ifndef PIC_H__
#define PIC_H__

#include <stdbool.h>

#define PIC_MASTER_INTERRUPT_BASE (0x20)
#define PIC_SLAVE_INTERRUPT_BASE (PIC_MASTER_INTERRUPT_BASE + 8)

enum
{
  PIC_NUM_PIT                   =  0,
  PIC_NUM_KEYBOARD_CONTROLLER_1 =  1,
  PIC_NUM_SLAVE                 =  2,
  PIC_NUM_RS232_2_4             =  3,
  PIC_NUM_RS232_1_3             =  4,
  PIC_NUM_LPT2                  =  5,
  PIC_NUM_FLOPPY                =  6,
  PIC_NUM_LPT1                  =  7,

  PIC_NUM_RTC                   =  8,
                                // 9,
  PIC_NUM_ATA_4                 = 10,
  PIC_NUM_ATA_3                 = 11,
  PIC_NUM_KEYBOARD_CONTROLLER_2 = 12,
  PIC_NUM_FPU                   = 13,
  PIC_NUM_ATA_1                 = 14,
  PIC_NUM_ATA_2                 = 15,
};

enum
{
  PIC_MASK_PIT                   = 1 << PIC_NUM_PIT,
  PIC_MASK_KEYBOARD_CONTROLLER_1 = 1 << PIC_NUM_KEYBOARD_CONTROLLER_1,
  PIC_MASK_SLAVE                 = 1 << PIC_NUM_SLAVE,
  PIC_MASK_RS232_2_4             = 1 << PIC_NUM_RS232_2_4,
  PIC_MASK_RS232_1_3             = 1 << PIC_NUM_RS232_1_3,
  PIC_MASK_LPT2                  = 1 << PIC_NUM_LPT2,
  PIC_MASK_FLOPPY                = 1 << PIC_NUM_FLOPPY,
  PIC_MASK_LPT1                  = 1 << PIC_NUM_LPT1,
                                            
  PIC_MASK_RTC                   = 1 << PIC_NUM_RTC,
                                // 9,
  PIC_MASK_ATA_4                 = 1 << PIC_NUM_ATA_4,
  PIC_MASK_ATA_3                 = 1 << PIC_NUM_ATA_3,
  PIC_MASK_KEYBOARD_CONTROLLER_2 = 1 << PIC_NUM_KEYBOARD_CONTROLLER_2,
  PIC_MASK_FPU                   = 1 << PIC_NUM_FPU,
  PIC_MASK_ATA_1                 = 1 << PIC_NUM_ATA_1,
  PIC_MASK_ATA_2                 = 1 << PIC_NUM_ATA_2,
};

bool pic_init (void);
void pic_mask (int mask);

#endif
