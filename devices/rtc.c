// Extracted from http://wiki.osdev.org/index.php?title=CMOS&oldid=12676
// by http://wiki.osdev.org/User:Bewing

#include "rtc.h"
#include <common/ports.h>
#include <common/intr.h>
#include <common/nop.h>

enum
{
  cmos_address = 0x70,
  cmos_data    = 0x71,
};

enum
{
  reg_second = 0x00,
  reg_minute = 0x02,
  reg_hour   = 0x04,
  reg_day    = 0x07,
  reg_month  = 0x08,
  reg_year   = 0x09,
  reg_update = 0x0a,
  reg_b      = 0x0b,
};

enum
{
  flag_24h = 0x02,
  flag_binary = 0x04,
};

static inline uint8_t
read (int reg) {
  outb (cmos_address, reg);
  return inb (cmos_data);
}

static inline bool
is_update_in_progress (void)
{
  return (read (reg_update) & 0x80) != 0;
}

bool
rtc_read (struct rtc_data *dest)
{
  bool old_intr_level = intr_get_and_set_off ();

  while (is_update_in_progress ())
    expensive_nop ();

  uint8_t second = read (reg_second);
  uint8_t minute = read (reg_minute);
  uint8_t hour   = read (reg_hour);
  uint8_t day    = read (reg_day);
  uint8_t month  = read (reg_month);
  uint8_t year   = read (reg_year);
  uint8_t b      = read (reg_b);

  if (!(b & flag_binary))
    {
      second = (second & 0x0F) + ((second / 16) * 10);
      minute = (minute & 0x0F) + ((minute / 16) * 10);
      hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
      day = (day & 0x0F) + ((day / 16) * 10);
      month = (month & 0x0F) + ((month / 16) * 10);
      year = (year & 0x0F) + ((year / 16) * 10);
    }
  if (!(b & flag_24h) && (hour & 0x80))
    hour = ((hour & 0x7F) + 12) % 24;

  dest->second = second;
  dest->minute = minute;
  dest->hour   = hour;
  dest->day    = day;
  dest->month  = month;
  dest->year   = year;

  intr_set (old_intr_level);
  return true;
}

uint8_t
rtc_read_second (void)
{
  while (is_update_in_progress ())
    expensive_nop ();

  uint8_t second = read (reg_second);
  uint8_t b      = read (reg_b);
  if (!(b & flag_binary))
    second = (second & 0x0F) + ((second / 16) * 10);

  return second;
}
