CFLAGS=-O2 -nostdlib -nostdinc -ffreestanding -I../include -std=c99 -Wall

catypes: catypes.c
	gcc catypes.c $(CFLAGS) ../linux/libca.a -o catypes
#	strip -s -R .comment -R .eh_frame -R .eh_frame_hdr catypes
clean:
	rm -f catypes
