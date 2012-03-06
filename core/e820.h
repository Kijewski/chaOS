#ifndef E820_H__
#define E820_H__

#include <attributes.h>
#include <stdint.h>

#define E820_BASE ((const struct e820_ref *) 0x4500)

enum
{
  E820_MEMORY = 1,
  E820_RESERVED = 2,
};

struct e820_entry
{
  uint64_t base_addr;
  uint64_t length;
  uint32_t type;
  char add_data[0];
} PACKED;

struct e820_ref
{
  uint32_t size;
  struct e820_entry entry;
} PACKED;

#endif
