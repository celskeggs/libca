ifndef BITS
ARCH=$(shell uname -m)
ifeq ($(ARCH),x86_64)
BITS=64
endif
ifeq ($(ARCH),i686)
BITS=32
endif
endif

echo=echo -e "\e[1;3$(1)m[$(2)] $(3)\e[0m"

DIRS=$(shell echo */)

ifdef BITS

.PHONY: all $(DIRS)

all: $(DIRS)

$(DIRS): %:
	@$(call echo,3,ATTEMPTING SUITE,$(@:/=))
	+@(cd $@ && make BITS=$(BITS)) || ($(call echo,1,FAILED SUITE,$(@:/=)) && exit 1)
	@$(call echo,2,PASSED SUITE,$(@:/=))

else

main:
	@echo 'BITS must be set!'
	@exit 1

endif

CLEAN_DIRS=$(addprefix clean_,$(DIRS))

.PHONY: clean $(CLEAN_DIRS)

clean: $(CLEAN_DIRS)

$(CLEAN_DIRS): clean_%:
	cd $(@:clean_%=%) && make clean
