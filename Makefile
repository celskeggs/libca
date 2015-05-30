include common.mk

.PHONY: main test clean

main:
	+cd $(PSRCDIR) && make

test:
	+cd $(TESTDIR) && make

clean:
	+cd $(PSRCDIR) && make clean
	+cd $(TESTDIR) && make clean

distclean: clean
	rm -f $(PSRCDIR)/config.mk config.mk config.status
