# BOOTLOADER
TARGET := build

NASM := nasm
NASMFLAGS := -fbin -Ox

all:: $(TARGET)/bootloader.bin

clean::
	rm -rf $(TARGET)

$(TARGET)/bootloader.bin: bootloader.s
	@mkdir -p $(TARGET)
	$(NASM) $(NASMFLAGS) -o $@ $<
