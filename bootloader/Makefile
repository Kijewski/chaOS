# BOOTLOADER
TARGET := build

NASM := nasm
NASMFLAGS := -fbin -Ox

ELF_FILES := second_stage
ASM_FILES := bootloader $(ELF_FILES)
D_FILES := 

all:: $(ASM_FILES:%=$(TARGET)/%.bin) $(ELF_FILES:%=$(TARGET)/%.elf)

clean::
	rm -rf $(TARGET)

$(TARGET)/%.elf: $(TARGET)/%.bin
	objcopy -B i386:x86-64 -I binary -O elf64-x86-64 $< $@

$(TARGET)/%.bin: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

$(TARGET)/%.d: %.asm
	@mkdir -p $(TARGET)
	$(NASM) $(NASMFLAGS) -M -MD $@ -MT $(@:.d=.o) $<

-include $(ASM_FILES:%=$(TARGET)/%.d)
