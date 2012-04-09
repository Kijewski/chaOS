; Crude composition of
;   * http://wiki.osdev.org/index.php?title=Talk:Entering_Long_Mode_Directly&oldid=12173
;     by http://wiki.osdev.org/User:Oculusfervoris
; and
;   * http://wiki.osdev.org/index.php?title=Unreal_Mode&oldid=10705
;     by http://wiki.osdev.org/User:Jhawthorn

KERNEL_BASE equ 1024*1024
ELF64_ENTRY_POINT equ 3*8

PARTITION equ 1

MAX_SECTORS_AT_ONCE equ 32
E820_MAP_BASE equ 0x0500 + 0x200*MAX_SECTORS_AT_ONCE ; 0x4500

SECOND_STAGE_BASE equ 0x0500
ELF64_HEADER_SIZE equ 0xB0
SECOND_STAGE_MAX_SIZE equ 0x1000
