CFLAGS=-O2 -m$(BITS) -nostdlib -nostdinc -ffreestanding -I../../include -std=c99 -Wall

TESTS_C=$(shell echo *_t.c)
# TESTS=$(shell echo *_t) $(TESTS_C:.c=)
TESTS=$(TESTS_C:.c=)
RUN_TESTS=$(addprefix test_, $(TESTS))

.PHONY: $(RUN_TESTS)

tests: ${RUN_TESTS}
	@echo "All succeeded!"

$(RUN_TESTS): test_%: %
	@echo "[RUNNING TEST] $<"
	@./$<
	@echo "Passed."

ifdef BITS

%: %.c
	gcc $< $(CFLAGS) ../../linux/libca.a -o $@

else

%: %.c
	@echo 'BITS must be set!'
	@exit 1

endif

clean:
	rm -f $(TESTS)
