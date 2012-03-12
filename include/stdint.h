#ifndef STDINT_H__
#define STDINT_H__

typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int      int32_t;
typedef signed long     int64_t;
typedef signed __int128 int128_t;

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long     uint64_t;
typedef unsigned __int128 uint128_t;

typedef int8_t   int_least8_t;
typedef int16_t  int_least16_t;
typedef int32_t  int_least32_t;
typedef int64_t  int_least64_t;
typedef int128_t int_least128_t;

typedef uint8_t   uint_least8_t;
typedef uint16_t  uint_least16_t;
typedef uint32_t  uint_least32_t;
typedef uint64_t  uint_least64_t;
typedef uint128_t uint_least128_t;

typedef int8_t   int_fast8_t;
typedef int16_t  int_fast16_t;
typedef int32_t  int_fast32_t;
typedef int64_t  int_fast64_t;
typedef int128_t int_fast128_t;

typedef uint8_t   uint_fast8_t;
typedef uint16_t  uint_fast16_t;
typedef uint32_t  uint_fast32_t;
typedef uint64_t  uint_fast64_t;
typedef uint128_t uint_fast128_t;

typedef int64_t  intptr_t;
typedef int128_t intmax_t;

typedef uint64_t  uintptr_t;
typedef uint128_t uintmax_t;

#endif
