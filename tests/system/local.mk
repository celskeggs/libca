TEST_TARGET_SOURCE = $(shell echo *_target.c)
TEST_TARGETS = $(addprefix $(TESTBUILDDIR)/,$(TEST_TARGET_SOURCE:.c=))

EXTRA_BUILDS += $(TEST_TARGETS)
EXTRA_CLEANS += $(TEST_TARGETS)