include ../common.mk
# assumption: we are in $(PSRCDIR)

ALL_CFLAGS += -I$(SRCDIR)/libsyscall-built

SRCS=$(shell echo *.c)

OBJS=$(addprefix $(BUILDDIR)/,$(SRCS:.c=.o) dlmalloc-2.8.6/interface.o)

LIBSYSCALL=$(SRCDIR)/libsyscall-built/libsyscall.a

$(BUILDDIR)/libca.a: $(OBJS) $(LIBSYSCALL)
	mkdir libsyscall
	cd libsyscall && ar x $(LIBSYSCALL)
	ar rcs $(BUILDDIR)/libca.a $(OBJS) libsyscall/*
	rm -rf libsyscall

$(BUILDDIR)/%.o: %.c $(INCDIR)/*.h
	mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS) $< -o $@ -c

clean:
	rm -f $(OBJS) $(BUILDDIR)/libca.a
	rm -rf libsyscall
