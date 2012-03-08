#ifndef KERNEL_H__
#define KERNEL_H__

#include <stdint.h>

extern uint8_t _section_text_start[0];
extern uint8_t _section_text_end[0];

extern uint8_t _section_rodata_start[0];
extern uint8_t _section_rodata_end[0];

extern uint8_t _section_data_start[0];
extern uint8_t _section_data_end[0];

extern uint8_t _section_bss_start[0];
extern uint8_t _section_bss_end[0];

extern uint8_t _kernel_memory_end[0];

#endif
