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

#define UINT8_MAX (0xFFu)
#define INT8_MAX  (0x7F)
#define INT8_MIN (-0x80)

#define UINT16_MAX (0xFFFFu)
#define INT16_MAX  (0x7FFF)
#define INT16_MIN (-0x8000)

#define UINT32_MAX (0xFFFFFFFFu)
#define INT32_MAX  (0x7FFFFFFF)
#define INT32_MIN (-0x80000000)

#define UINT64_MAX (0xFFFFFFFFFFFFFFFFull)
#define INT64_MAX  (0x7FFFFFFFFFFFFFFFll)
#define INT64_MIN (-0x8000000000000000ll)

#define UINT128_MAX (((uint128_t) UINT64_MAX << 64) + UINT64_MAX)
#define INT128_MAX  ((int128_t) ((((uint128_t) 0x8000000000000000ull)<<64) - 1))
#define INT128_MIN  (-INT128_MAX - 1)

#define UINTPTR_MAX (UINT64_MAX)
#define INTPTR_MIN (INT64_MIN)
#define INTPTR_MAX (INT64_MAX)

#define UINTMAX_MAX (UINT128_MAX)
#define INTMAX_MIN (INT128_MIN)
#define INTMAX_MAX (INT128_MAX)

#define PTRDIFF_MIN (INT64_MIN)
#define PTRDIFF_MAX (INT64_MAX)

#define SIZE_MAX (UINT64_MAX)

#endif
