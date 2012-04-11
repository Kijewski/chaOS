#ifndef ASSERT_H__
#define ASSERT_H__

#include <common/attributes.h>
#include <stddef.h>

#define CASSERT_CONCAT_(X,Y) X##Y
#define CASSERT_CONCAT(X,Y) CASSERT_CONCAT_(X,Y)

#define LOCAL_CASSERT(X)                                                      \
    ((void) sizeof (struct {                                                  \
      int CASSERT_CONCAT (a_statical_assertion_was_not_met_in_line_,          \
                          __LINE__) : -1 + 2*!!(X);                           \
    }))

#define GLOBAL_CASSERT(X)                                                     \
    static inline void ALWAYS_INLINE DEPRECATED                               \
    CASSERT_CONCAT (__cassert_, __COUNTER__) (void)                           \
    {                                                                         \
      LOCAL_CASSERT ((X));                                                    \
    }

void assert_failure (const char *expr, const char *file, int line) NO_RETURN;

#define ENSURE(X)                                                             \
    do                                                                        \
      {                                                                       \
        if (UNLIKELY (!(X)))                                                  \
          assert_failure (#X, __FILE__, __LINE__);                            \
      }                                                                       \
    while (0)

#define assert(X) ASSERT (X)

#endif

#undef ASSERT

#ifndef NDEBUG
# define ASSERT(X) ENSURE (X)
#else
# define ASSERT(X) do { break; } while (0)
#endif
