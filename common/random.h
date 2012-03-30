#ifndef __COMMON_RANDOM_H
#define __COMMON_RANDOM_H

#include <stdbool.h>
#include <stdint.h>

bool random_init (void);

uint64_t random_get (void);

#endif
