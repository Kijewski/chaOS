#ifndef KERNEL_H__
#define KERNEL_H__

#include <stdint.h>

extern const uintptr_t _section_text_start;
extern const uintptr_t _section_text_end;

extern const uintptr_t _section_rodata_start;
extern const uintptr_t _section_rodata_end;

extern const uintptr_t _section_data_start;
extern const uintptr_t _section_data_end;

extern const uintptr_t _section_bss_start;
extern const uintptr_t _section_bss_end;

extern const uintptr_t _kernel_memory_end;

#endif
