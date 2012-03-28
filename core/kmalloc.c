#include "kmalloc.h"
#include "paging.h"
#include "frame_allocator.h"

bool
kmalloc_init (void)
{
  return true; // TODO
}

void * __attribute__ ((malloc))
kmalloc (size_t size, bool nx)
{
  if (size == 0)
    return NULL;

  return NULL; // TODO
  (void) size;
  (void) nx;
}

void * __attribute__ ((malloc))
krealloc (void *ptr, size_t new_size, bool nx)
{
  if (new_size == 0)
    {
      kfree (ptr);
      return NULL;
    }
  if (ptr == NULL)
    return kmalloc (new_size, nx);

  return NULL; // TODO
  (void) ptr;
  (void) new_size;
  (void) nx;
}

void
kfree (void *ptr)
{
  if (ptr == NULL)
    return;

  // TODO
  (void) ptr;
}
