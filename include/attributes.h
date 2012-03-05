#ifndef ATTRIBUTES_H__
#define ATTRIBUTES_H__

#define NO_RETURN __attribute__ ((noreturn))
#define PACKED __attribute__ ((packed))

#define USED __attribute__ ((used))
#define UNUSED __attribute__ ((unused))
#define DEPRECATED __attribute__ ((deprecated))

#define NO_INLINE __attribute__ ((noinline))
#define ALWAYS_INLINE __attribute__ ((always_inline))
#define GNU_INLINE __attribute__ ((gnu_inline))
#define INLINE_ONLY(...) extern inline __VA_ARGS__ GNU_INLINE ALWAYS_INLINE

#define PURE __attribute__ ((pure))
#define CONST __attribute__ ((const))

#define CDECL __attribute__ ((cdecl))
#define FASTCALL __attribute__ ((fastcall))
#define STDCALL __attribute__ ((stdcall))

#define MALLOC __attribute__ ((malloc))

#define NON_NULL(...) __attribute__ ((nonnull (__VA_ARGS__)))
#define SENTINEL __attribute__ ((sentinel))

#define ALIAS(X) __attribute__ ((alias (#X)))
#define WEAK __attribute__ ((weak))

#define ALIGNED(X) __attribute__ ((aligned (X)))
#define OPTIMIZE(X) __attribute__ ((optimize (X)))

#define HOT __attribute__ ((hot))
#define COLD __attribute__ ((cold))

#endif
