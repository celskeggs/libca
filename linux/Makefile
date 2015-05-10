CFLAGS=-nostdlib -nostdinc -std=c99 -I../include -I../libsyscall-built -fno-builtin

libca.a: begin.o
	mkdir libsyscall
	cd libsyscall && ar x ../../libsyscall-built/libsyscall.a
	ar rcs libca.a begin.o libsyscall/*
	rm -rf libsyscall

begin.o: begin.c ../include/*.h
	gcc $(CFLAGS) begin.c -o begin.o -c

clean:
	rm -f begin.o libca.a
	rm -rf libsyscall
