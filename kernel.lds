OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

SECTIONS
{
  . = 1024*1024 + SIZEOF_HEADERS;

  .text :
  {
    _section_text_start = .;
    . = ALIGN(0x1000);
    *(.text)
    *(.text.*)
    . = ALIGN(0x1000);
    _section_text_end = .;
  }

  .rodata :
  {
    _section_rodata_start = .;
    . = ALIGN(0x1000);
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(0x1000);
    _section_rodata_end = .;
  }

  .data :
  {
    _section_data_start = .;
    . = ALIGN(0x1000);
    *(.data)
    *(.data.*)
    . = ALIGN(0x1000);
    _section_data_end = .;
  }

  .bss :
  {
    _section_bss_start = .;
    . = ALIGN(0x1000);
    *(.bss)
    *(.bss.*)
    . = ALIGN(0x1000);
    _section_bss_end = .;
  }
}
