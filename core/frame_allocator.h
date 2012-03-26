#ifndef FRAME_ALLOCATOR_H__
#define FRAME_ALLOCATOR_H__

#include <stdbool.h>
#include <stdint.h>

bool frame_allocator_init (void);

void *frame_allocator_get (void);
bool frame_allocator_return (void *);

uint64_t free_frames_count (void);

#endif
