#ifndef ITOA_H__
#define ITOA_H__

#include <stdint.h>
#include <attributes.h>

extern const char ITOA_ALPHABET[];

// the numbers include the nullterminator and a minus
#define I64_TO_OCT_BUF_SIZE (22 + 2)
#define I64_TO_DEC_BUF_SIZE (20 + 2)
#define I64_TO_HEX_BUF_SIZE (16 + 2)
#define I64_TO_B32_BUF_SIZE (13 + 2)
#define I64_TO_B36_BUF_SIZE (13 + 2) /* sic */

char *_i64_to_base (int64_t v, char *c, const char *H, int base);

// The functions write the string to the end of buf!
// The functions return the pointer to the start of the translated string.

static inline char *
i64_to_oct (int64_t v, char *buf)
{
  return _i64_to_base (v, &buf[I64_TO_OCT_BUF_SIZE-1], &ITOA_ALPHABET[27], 8);
}

static inline char *
i64_to_dec (int64_t v, char *buf)
{
  return _i64_to_base (v, &buf[I64_TO_DEC_BUF_SIZE-1], &ITOA_ALPHABET[25], 10);
}

static inline char *
i64_to_hex (int64_t v, char *buf)
{
  return _i64_to_base (v, &buf[I64_TO_HEX_BUF_SIZE-1], &ITOA_ALPHABET[35], 16);
}

static inline char *
i64_to_b32 (int64_t v, char *buf)
{
  return _i64_to_base (v, &buf[I64_TO_B32_BUF_SIZE-1], &ITOA_ALPHABET[3], 32);
}

static inline char *
i64_to_b36 (int64_t v, char *buf)
{
  return _i64_to_base (v, &buf[I64_TO_B36_BUF_SIZE-1], &ITOA_ALPHABET[0], 36);
}

// TODO: implement
#define U64_TO_HEX_BUF_SIZE I64_TO_HEX_BUF_SIZE
#define u64_to_hex i64_to_hex

#endif
