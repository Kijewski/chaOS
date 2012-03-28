#ifndef __COMMON_GCC_HELPERS_H
#define __COMMON_GCC_HELPERS_H

#include <stdint.h>

uint128_t __umodti3 (uint128_t num, uint128_t den);
uint128_t __udivti3 (uint128_t num, uint128_t den);
uint128_t __udivmodti4(uint128_t num, uint128_t den, uint128_t *rem_p);

#endif
