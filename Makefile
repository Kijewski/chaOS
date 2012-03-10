# IMAGE

TARGET := build

LD := ld
LDFLAGS := --omagic

FOLDERS := core devices include common
ARCHIVES := $(FOLDERS:%=%/$(TARGET)/build.a)

all:: $(TARGET)/disk.img

clean:: $(FOLDERS:%=.clean.%)
	rm -rf $(TARGET)

$(FOLDERS:%=.clean.%) .clean.bootsector:
	cd $(@:.clean.%=%) && $(MAKE) clean

$(ARCHIVES): %/$(TARGET)/build.a: % .PHONY
	cd $< && $(MAKE) all

bootloader/$(TARGET)/bootloader.bin: bootloader .PHONY
	cd $< && $(MAKE) all

$(TARGET)/kernel.bin: kernel.lds $(ARCHIVES)
	@mkdir -p $(TARGET)
	$(LD) $(LDFLAGS) -Map $@.map -T kernel.lds -o $@ --whole-archive $(ARCHIVES)

$(TARGET)/disk.img: $(TARGET)/kernel.bin bootloader/$(TARGET)/bootloader.bin
	dd if=/dev/zero of=$@ bs=4k count=1k
	parted -s $@ -- mklabel msdos
	parted -s -a minimal $@ -- mkpart primary ext2 \
			1s  $(shell dc -e "$(shell stat -c%s $(TARGET)/kernel.bin) 4095+4096/8*p")s
	parted -s -a minimal $@ -- mkpart primary fat16 \
			$(shell dc -e "$(shell stat -c%s $(TARGET)/kernel.bin) 4095+4096/8*1+p")s 100%
	#parted -s $@ -- set 1 boot on
	dd conv=notrunc if=bootloader/$(TARGET)/bootloader.bin of=$@ bs=446 count=1
	dd conv=notrunc if=$(TARGET)/kernel.bin of=$@ bs=512 seek=1

.PHONY:
