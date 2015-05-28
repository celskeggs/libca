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
CFLAGS=-O2 -m$(BITS) -nostdlib -nostdinc -std=c99 -I../include -I../libsyscall-built -fno-builtin -Wall

OBJS=begin.o panic.o stream.o string.o alloc.o memory.o file.o dlmalloc-2.8.6/interface.o

LIBSYSCALL=../libsyscall-built/libsyscall.a

libca.a: $(OBJS) $(LIBSYSCALL)
	mkdir libsyscall
	cd libsyscall && ar x ../$(LIBSYSCALL)
	ar rcs libca.a $(OBJS) libsyscall/*
	rm -rf libsyscall

%.o: %.c ../include/*.h
	gcc $(CFLAGS) $< -o $@ -c

else

main:
	@echo 'BITS must be set!'
	@exit 1

endif
clean:
	rm -f $(OBJS) libca.a
	rm -rf libsyscall
