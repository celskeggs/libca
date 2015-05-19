CFLAGS=-nostdlib -nostdinc -std=c99 -I../include -I../libsyscall-built -fno-builtin -Wall -ggdb

OBJS=begin.o panic.o stream.o string.o alloc.o memory.o file.o dlmalloc-2.8.6/interface.o

libca.a: $(OBJS)
	mkdir libsyscall
	cd libsyscall && ar x ../../libsyscall-built/libsyscall.a
	ar rcs libca.a $(OBJS) libsyscall/*
	rm -rf libsyscall

%.o: %.c ../include/*.h
	gcc $(CFLAGS) $< -o $@ -c

clean:
	rm -f $(OBJS) libca.a
	rm -rf libsyscall
