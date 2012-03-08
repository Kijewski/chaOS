#ifndef KERNEL_H__
#define KERNEL_H__

extern void _section_text_start;
extern void _section_text_end;

extern void _section_rodata_start;
extern void _section_rodata_end;

extern void _section_data_start;
extern void _section_data_end;

extern void _section_bss_start;
extern void _section_bss_end;

extern void _kernel_memory_end;

#endif
