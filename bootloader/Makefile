# BOOTLOADER
TARGET := build

NASM := nasm
NASMFLAGS := -fbin -Ox

all:: $(TARGET)/bootloader.bin

clean::
	rm -rf $(TARGET)

$(TARGET)/bootloader.bin: bootloader.s
	$(NASM) $(NASMFLAGS) -o $@ $<

$(TARGET)/bootloader.d: bootloader.s
	@mkdir -p $(TARGET)
	$(NASM) $(NASMFLAGS) -M -MD $@ -MT $(@:.d=.o) $<

-include $(TARGET)/bootloader.d
