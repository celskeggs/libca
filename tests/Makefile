ifndef BITS
ARCH=$(shell uname -m)
ifeq ($(ARCH),x86_64)
BITS=64
endif
ifeq ($(ARCH),i686)
BITS=32
endif
endif

ifdef BITS
CFLAGS=-O2 -m$(BITS) -nostdlib -nostdinc -ffreestanding -I../include -std=c99 -Wall

catypes: catypes.c
	gcc catypes.c $(CFLAGS) ../linux/libca.a -o catypes
#	strip -s -R .comment -R .eh_frame -R .eh_frame_hdr catypes

else

main:
	@echo 'BITS must be set!'
	@exit 1

endif

clean:
	rm -f catypes
