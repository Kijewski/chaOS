#ifndef INTR_H__
#define INTR_H__

#include "attributes.h"
#include <stdbool.h>
#include <stdint.h>

static inline bool FUNCTION
intr_get (void)
{
  uint64_t r;
  asm volatile ("pushf;"
                "pop %0"
                : "=g"(r));
  return r & (1 << 9);
}

static inline void
intr_set (bool on)
{
  if (on)
    asm volatile ("sti");
  else
    asm volatile ("cli");
}

static inline bool FUNCTION
intr_get_and_set_on (void)
{
  bool r = 0;
  asm volatile ("pushfq;"
                "btsq $9, (%%rsp);"
                "adc $0, %0;"
                "popfq;"
                : "+g"(r));
  return r;
}

static inline bool FUNCTION
intr_get_and_set_off (void)
{
  bool r = 0;
  asm volatile ("pushfq;"
                "btrq $9, (%%rsp);"
                "adc $0, %0;"
                "popf;"
                : "+g"(r));
  return r;
}

static inline bool FUNCTION
intr_get_and_set_compl (void)
{
  bool r = 0;
  asm volatile ("pushfq;"
                "btcq $9, (%%rsp);"
                "adc $0, %0;"
                "popfq;"
                : "+g"(r));
  return r;
}

#endif
