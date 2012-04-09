#ifndef __DEVICES_RTC_H
#define __DEVICES_RTC_H

#include <stdint.h>
#include <stdbool.h>
#include <core/interrupts.h>

enum
{
  RTC_INTR_NUM = 0x70,
};

struct rtc_data
{
  uint8_t second, minute, hour;
  uint8_t day, month, year;
};

bool rtc_init (void);

bool rtc_read (struct rtc_data *dest);

uint8_t rtc_read_second (void);

typedef void rtc_intr_handler_fun (bool                    is_alarm,
                                   struct interrupt_frame *f,
                                   void                   *aux);

bool rtc_set_interrupt (struct rtc_data      *date,
                        rtc_intr_handler_fun *fun,
                        void                 *aux);
bool rtc_remove_interrupt (void);

#endif
