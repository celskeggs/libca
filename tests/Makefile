include ../common.mk

DIRS=$(shell echo */)
CLEAN_DIRS=$(addprefix clean_,$(DIRS))

all: $(DIRS)

$(DIRS): %:
	@$(call echo,3,ATTEMPTING SUITE,$(@:/=))
	+@(cd $@ && make TESTSUITE=$(@:/=)) || ($(call echo,1,FAILED SUITE,$(@:/=)) && exit 1)
	@$(call echo,2,PASSED SUITE,$(@:/=))

.PHONY: all clean $(DIRS) $(CLEAN_DIRS)

clean: $(CLEAN_DIRS)

$(CLEAN_DIRS): clean_%:
	cd $(@:clean_%=%) && make clean
