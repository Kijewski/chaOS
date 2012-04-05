#ifndef __COMMON_INT128_H
#define __COMMON_INT128_H

typedef signed __int128 int128_t;
typedef unsigned __int128 uint128_t;
typedef int128_t int_least128_t;
typedef uint128_t uint_least128_t;
typedef int128_t int_fast128_t;
typedef uint128_t uint_fast128_t;

#define UINT128_MAX (((uint128_t) UINT64_MAX << 64) + UINT64_MAX)
#define INT128_MAX  ((int128_t) ((((uint128_t) 0x8000000000000000ull)<<64) - 1))
#define INT128_MIN  (-INT128_MAX - 1)

#endif
