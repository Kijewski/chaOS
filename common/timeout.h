#ifndef __COMMON_TIMEOUT_H
#define __COMMON_TIMEOUT_H

#include <stdbool.h>
#include <stdint.h>

bool timeout_init (void);

void timeout_s (uint64_t duration);
void timeout_ms (uint64_t duration);
void timeout_ns (uint64_t duration); // unreliable!

#endif
