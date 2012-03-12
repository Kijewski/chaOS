#ifndef BSWAP_H__
#define BSWAP_H__

#include <stdint.h>
#include <attributes.h>
#include <round.h>

static inline uint16_t
bswap16 (uint16_t value_)
{
  union {
    struct {
      uint8_t l, h;
    } PACKED;
    uint16_t v;
  } value = { .v = value_ };
  SWAP (value.l, value.h);
  return value.v;
}

static inline uint32_t
bswap32 (uint32_t value)
{
  asm ("bswap %0" : "+r"(value));
  return value;
}

static inline uint64_t
bswap64 (uint64_t value)
{
  asm ("bswap %0" : "+r"(value));
  return value;
}

static inline uint64_t
bswap48 (uint64_t value)
{
  return bswap64 (value << 16);
}

static inline uint128_t
bswap128 (uint128_t value_)
{
  union {
    struct {
      uint64_t l, h;
    } PACKED;
    uint128_t v;
  } value = { .v = value_ };
  value.l = bswap64 (value.l);
  value.h = bswap64 (value.h);
  SWAP (value.l, value.h);
  return value.v;
}

#endif
