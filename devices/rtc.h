#ifndef __DEVICES_RTC_H
#define __DEVICES_RTC_H

#include <stdint.h>
#include <stdbool.h>

struct rtc_data
{
  uint8_t second, minute, hour;
  uint8_t day, month, year;
};

bool rtc_read (struct rtc_data *dest);

uint8_t rtc_read_second (void);

#endif
