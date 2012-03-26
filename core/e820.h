#ifndef E820_H__
#define E820_H__

#include <attributes.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

enum
{
  E820_MEMORY = 1,
  E820_RESERVED = 2,
  E820_ACPI_RECLAIMABLE = 3,
  E820_ACPI_NVS = 4,
  E820_BAD = 5,
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
  uint16_t size;
  struct e820_entry entry;
} PACKED;

static inline const struct e820_ref *
e820_start (void)
{
  const struct e820_ref *result = (void *) 0x4500;
  return result->size != 0 ? result : NULL;
}

static inline const struct e820_ref *
e820_next (const struct e820_ref *cur)
{
  ASSERT (cur->size != 0);
  const struct e820_ref *result;
  result = (void *) ((uintptr_t) cur + cur->size + sizeof (cur->size));
  return result->size != 0 ? result : NULL;
}

#endif
