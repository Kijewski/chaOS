#include "cpuid.h"

// -fPIC fix is curtesy of Antti Tuppurainen
// http://sourceforge.net/tracker/?func=detail&aid=3460083&group_id=14481&atid=114481

static inline void
cpuid (uint32_t num, void *a, void *b, void *c, void *d)
{
  typedef char _c[4];
#ifdef __PIC__
  asm ("mov %%rbx, %%rdi;\n"
       "cpuid;\n"
       "xchg %%rbx, %%rdi"
       : "=a"(*(_c *) a),
         "=D"(*(_c *) b),
         "=c"(*(_c *) c),
         "=d"(*(_c *) d)
       : "a"(num));
#else
  asm ("cpuid;"
       : "=a"(*(_c *) a),
         "=b"(*(_c *) b),
         "=c"(*(_c *) c),
         "=d"(*(_c *) d)
       : "a"(num));
#endif
}

void
cpuid_basic_info (struct cpuid_basic_info *result)
{
  cpuid (0x00000000,
         &result->max_cpuid,
         &result->vendor_string[0*4],
         &result->vendor_string[1*4],
         &result->vendor_string[2*4]);
}

void
cpuid_processor_brand_string (struct cpuid_processor_brand_string *result)
{
  cpuid (0x80000002,
         &result->string[0*4],
         &result->string[1*4],
         &result->string[2*4],
         &result->string[3*4]);
  cpuid (0x80000003,
         &result->string[4*4],
         &result->string[5*4],
         &result->string[6*4],
         &result->string[7*4]);
  cpuid (0x80000004,
         &result->string[8*4],
         &result->string[9*4],
         &result->string[10*4],
         &result->string[11*4]);
}

uint64_t
cpuid_features (void)
{
  uint32_t eax, ebx, ecx, edx;
  cpuid (0x00000001, &eax, &ebx, &ecx, &edx);
  return ((uint64_t) edx << 32) + ecx;
}
