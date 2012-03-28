#ifndef CTYPE_H__
#define CTYPE_H__

#include <attributes.h>

// TODO

#define _IN_RANGE(X, A, B)       \
    ({                           \
      const typeof (X) _x = (X); \
      _x >= (A) && _x <= (B);    \
    })

#define isalnum(X)                  \
    ({                              \
      const typeof (X) _x = (X);    \
      isalpha (_x) || isdigit (_x); \
    })
#define isalpha(X)                  \
    ({                              \
      const typeof (X) _x = (X);    \
      islower (_x) || isupper (_x); \
    })
#define isascii(X) (_IN_RANGE ((X), 0, 0x7F))
#define iscntrl(X)                           \
    ({                                       \
      const typeof (X) _x = (X);             \
      _IN_RANGE (_x, 0, 0x1F) || _x == 0x7F; \
    })
#define isdigit(X) (_IN_RANGE ((X), '0', '9'))
#define isgraph(X)                  \
    ({                              \
      const typeof (X) _x = (X);    \
      isalnum (_x) || ispunct (_x); \
    })
#define islower(X) (_IN_RANGE ((X), 'a', 'z'))
#define isprint(X)               \
    ({                           \
      const typeof (X) _x = (X); \
      isgraph (_x) || _x == 'x'; \
    })
#define ispunct(X)                \
    ({                            \
      const typeof (X) _x = (X);  \
      !isalnum (_x) && _x != ' '; \
    })
#define isspace(X)                                                      \
    ({                                                                  \
      const typeof (X) _x = (X);                                        \
      _x == ' ' || _x == '\t' || _x == '\v' || _x == '\n' || _x == '\r' \
                || _x == '\f'                                           \
    })
#define isupper(X) (_IN_RANGE ((X), 'A', 'Z'))
#define isxdigit(X)                                                        \
    ({                                                                     \
      const typeof (X) _x = (X);                                           \
      isdigit (_x) || _IN_RANGE (_x, 'a', 'f') || _IN_RANGE (_x, 'A', 'F') \
    })
int   tolower(int);
#define tolower(X)               \
    ({                           \
      const typeof (X) _x = (X); \
      isupper (_x) ? _x|32 : _x; \
    })
#define toupper(X)                \
    ({                            \
      const typeof (X) _x = (X);  \
      islower (_x) ? _x&~32 : _x; \
    })

int   toascii(int) DEPRECATED;
int   _toupper(int) DEPRECATED;
int   _tolower(int) DEPRECATED;

#endif
