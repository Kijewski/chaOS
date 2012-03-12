OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

SECTIONS
{
  . = 1024*1024 + SIZEOF_HEADERS;

  .text :
  {
    . = ALIGN(0x1000);
    _section_text_start = .;
    *(.text)
    *(.text.*)
    . = ALIGN(0x1000);
    _section_text_end = .;
  }

  .rodata :
  {
    . = ALIGN(0x1000);
    _section_rodata_start = .;
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(0x1000);
    _section_rodata_end = .;
  }

  .data :
  {
    . = ALIGN(0x1000);
    _section_data_start = .;
    *(.data)
    *(.data.*)
    . = ALIGN(0x1000);
    _section_data_end = .;
  }

  .bss :
  {
    . = ALIGN(0x1000);
    _section_bss_start = .;
    *(.bss)
    *(.bss.*)
    . = ALIGN(0x1000);
    _section_bss_end = .;
  }

  _kernel_memory_end = .;
}
