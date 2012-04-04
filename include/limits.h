#ifndef __INCLUDE_LIMITS_H
#define __INCLUDE_LIMITS_H

#include <stdint.h>

#define CHAR_BIT   (8)
#define SCHAR_MIN  (INT8_MIN)
#define SCHAR_MAX  (INT8_MAX)
#define UCHAR_MAX  (UINT8_MAX)
#define CHAR_MIN   (SCHAR_MIN)
#define CHAR_MAX   (SCHAR_MAX)
/*#define MB_LEN_MAX (2)*/
#define SHRT_MIN   (INT16_MIN)
#define SHRT_MAX   (INT8_MAX)
#define USHRT_MAX  (UINT8_MAX)
#define INT_MIN    (INT32_MIN)
#define INT_MAX    (INT32_MAX)
#define UINT_MAX   (UINT32_MAX)
#define LONG_MIN   (INT32_MIN)
#define LONG_MAX   (INT32_MAX)
#define ULONG_MAX  (UINT32_MAX)

#define LONG_LONG_MIN  (INT64_MIN)
#define LONG_LONG_MAX  (INT64_MAX)
#define ULONG_LONG_MAX (UINT64_MAX)

#endif
