#ifndef KERNEL_H__
#define KERNEL_H__

extern const void _section_text_start;
extern const void _section_text_end;

extern const void _section_rodata_start;
extern const void _section_rodata_end;

extern const void _section_data_start;
extern const void _section_data_end;

extern const void _section_bss_start;
extern const void _section_bss_end;

extern const void _kernel_memory_end;

#endif
