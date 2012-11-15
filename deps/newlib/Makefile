NEWLIB_CWD := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(NEWLIB_CWD)/../../Makefile.vars
TARGET := $(NEWLIB_CWD)/build

CVS := cvs
CVSFLAGS := -z 7

CFLAGS += -w

export CC
export CFLAGS

ICONV_ENCODINGS := us_ascii,iso_8859_15,win_1252,utf_8,utf_16,ucs_2,ucs_4

all: $(TARGET)/lib/libc.a

$(TARGET)/lib/libc.a: src/newlib/config.status
	cd src/newlib && $(MAKE)
	cd src/newlib && $(MAKE) install

src/newlib/configure:
	$(CVS) $(CVSFLAGS) -d :pserver:anoncvs@sources.redhat.com:/cvs/src co newlib

src/newlib/config.status: src/newlib/configure
	cd src/newlib && ./configure --prefix=$(TARGET) \
	                             --enable-newlib-io-pos-args \
	                             --enable-newlib-io-c99-formats \
	                             --enable-newlib-register-fini \
	                             --enable-newlib-io-long-long \
	                             --enable-newlib-io-long-double \
	                             --enable-newlib-mb \
	                             --enable-newlib-iconv-encodings=$(ICONV_ENCODINGS) \
	                             --enable-multilib \
	                             --enable-malloc-debugging \
	                             --enable-newlib-multithread \
	                             --enable-newlib-iconv \
	                             --enable-static \
	                             --disable-shared \
	                             --enable-newlib-supplied-syscalls \
	                             --disable-newlib-hw-fp

clean:: src/newlib/configure
	rm -rf $(TARGET)
	rm -f src/newlib/config.status
	touch src/newlib/config.status
	chmod +x src/newlib/config.status
	cd src/newlib && make clean distclean
	rm -f src/newlib/config.status
