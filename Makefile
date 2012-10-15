# IMAGE

include Makefile.vars

LD := ld
LDFLAGS := --nmagic --fatal-warnings -nostdlib -static --whole-archive

FOLDERS := common core devices glue include
ARCHIVES := $(FOLDERS:%=%/$(TARGET)/build.a)

all:: $(TARGET)/disk.img

deps::
	cd deps && $(MAKE) all

$(DEPS_ACHIVES) :: deps

clean:: $(FOLDERS:%=.clean.%)
	rm -rf $(TARGET)
	cd deps && $(MAKE) clean

bochs:: $(TARGET)/disk.img
	bochs -q -f execute.bxrc

qemu:: $(TARGET)/disk.img
	qemu-system-x86_64 -cpu qemu64 -monitor stdio -m 32 -s -hda $<

$(FOLDERS:%=.clean.%) .clean.bootsector:
	cd $(@:.clean.%=%) && $(MAKE) clean

$(ARCHIVES): %/$(TARGET)/build.a: % deps
	cd $< && $(MAKE) all

bootloader/build/second_stage.elf bootloader/$(TARGET)/bootloader.bin: bootloader
	cd $< && $(MAKE) all

$(TARGET)/kernel.bin: kernel.lds $(ARCHIVES) $(DEPS_ACHIVES) bootloader/build/second_stage.elf
	@mkdir -p $(TARGET)
	$(LD) $(LDFLAGS) -Map $@.map -T kernel.lds -o $@ $(ARCHIVES) $(DEPS_ACHIVES:%=$(ROOT)/%)
	objcopy --only-keep-debug $@ $@.dbg
	strip -sx $@

$(TARGET)/disk.img: $(TARGET)/kernel.bin bootloader/$(TARGET)/bootloader.bin
	@rm -f $@
	bximage -q -hd -mode=flat -size=4 $@
	parted -s $@ -- mklabel msdos
	parted -s -a minimal $@ -- mkpart primary ext2 \
			1s  $(shell dc -e "$(shell stat -c%s $(TARGET)/kernel.bin) 4095+4096/8*p")s
	parted -s -a minimal $@ -- mkpart primary fat16 \
			$(shell dc -e "$(shell stat -c%s $(TARGET)/kernel.bin) 4095+4096/8*1+p")s 100%
	parted -s $@ -- set 1 boot on
	dd conv=notrunc if=bootloader/$(TARGET)/bootloader.bin of=$@ bs=446 count=1
	dd conv=notrunc if=$(TARGET)/kernel.bin of=$@ bs=512 seek=1
