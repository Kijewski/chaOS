#ifndef KMALLOC__
#define KMALLOC__

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <attributes.h>

bool kmalloc_init (void);

void *kmalloc (size_t size, bool nx) __attribute__ ((malloc));
void *krealloc (void *ptr, size_t new_size, bool nx) __attribute__ ((malloc));
void kfree (void *ptr);

#endif
