CFLAGS=-nostdlib -nostdinc -std=c99 -I../include -I../libsyscall-built -fno-builtin

libca.a: begin.o panic.o
	mkdir libsyscall
	cd libsyscall && ar x ../../libsyscall-built/libsyscall.a
	ar rcs libca.a begin.o panic.o libsyscall/*
	rm -rf libsyscall

%.o: %.c ../include/*.h
	gcc $(CFLAGS) $< -o $@ -c

clean:
	rm -f begin.o libca.a
	rm -rf libsyscall
