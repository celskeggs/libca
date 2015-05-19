CFLAGS=catypes.c -nostdlib -nostdinc -ffreestanding -I../include -std=c99 -Wall -ggdb

catypes: catypes.c
	gcc $(CFLAGS) ../linux/libca.a -o catypes
clean:
	rm -f catypes
