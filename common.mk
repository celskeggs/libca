TOP := $(dir $(lastword $(MAKEFILE_LIST)))
include $(TOP)/config.mk

CFLAGS=-O2 -ggdb -Wall
ALL_CFLAGS=-m$(BITS) -nostdlib -nostdinc -std=c99 -I$(INCDIR) -I$(SRCDIR)/libsyscall-built -fno-builtin $(CFLAGS)

echo=/bin/echo -e "\e[1;3$(1)m[$(2)] $(3)\e[0m"
