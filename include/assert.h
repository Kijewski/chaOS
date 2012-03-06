#ifndef ASSERT_H__
#define ASSERT_H__

#include <attributes.h>
#include <stddef.h>

#define CASSERT_CONCAT_(X,Y) X##Y
#define CASSERT_CONCAT(X,Y) CASSERT_CONCAT_(X,Y)

#define CASSERT_(X, BEFORE, AFTER)                                            \
    BEFORE inline size_t AFTER ALWAYS_INLINE DEPRECATED                       \
    CASSERT_CONCAT (__cassert_, __COUNTER__) (void)                           \
    {                                                                         \
      return sizeof (struct {                                                 \
        int a_statical_assertion_was_not_matched : -1 + 2*!!(X);              \
      });                                                                     \
    }
#define LOCAL_CASSERT(X) CASSERT_ (X, , )
#define GLOBAL_CASSERT(X) CASSERT_ (X, extern, GNU_INLINE)

void assert_failure (const char *expr, const char *file, int line);

#define ENSURE(X)                                                             \
    do                                                                        \
      {                                                                       \
        if (UNLIKELY (!(X)))                                                  \
          assert_failure (#X, __FILE__, __LINE__);                            \
      }                                                                       \
    while (0)

#endif

#undef ASSERT

#ifdef NDEBUG
# define ASSERT(X) ENSURE (X)
#else
# define ASSERT(X) do { break; } while (0)
#endif
