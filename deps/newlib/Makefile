NEWLIB_CWD := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
ROOT := $(NEWLIB_CWD)/../..
TARGET := $(NEWLIB_CWD)/build

CVS := cvs
CVSFLAGS := -z 7

export CC := gcc
export CFLAGS = -m64 -g3 -ggdb -mcmodel=small -fno-pic -mno-red-zone -fno-stack-protector \
                -fstrict-aliasing -fsigned-char -fno-common \
                -ffreestanding -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs \
                -I$(ROOT)/include -I$(ROOT)/common -I$(ROOT)

all:: src/newlib/config.status
	cd src/newlib && $(MAKE)
	cd src/newlib && $(MAKE) install

src/newlib/configure:
	$(CVS) $(CVSFLAGS) -d :pserver:anoncvs@sources.redhat.com:/cvs/src co newlib

src/newlib/config.status: src/newlib/configure
	cd src/newlib && ./configure --prefix=$(TARGET)                             \
	                             --enable-static                                \
	                             --disable-shared                               \
	                             --disable-newlib-supplied-syscalls             \
	                             --disable-newlib-io-float                      \
	                             --enable-newlib-io-c99-formats                 \
	                             --enable-newlib-io-pos-args

clean:: src/newlib/configure
	rm -rf $(TARGET)
	rm -f src/newlib/config.status
