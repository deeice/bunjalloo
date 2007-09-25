.PHONY: all clean

SCONS := $(shell which scons)
ifeq ($(strip $(SCONS)),)
$(error "This program uses the scons software construction tool. See www.scons.org for more details.")
endif

all:
	$(SCONS)

clean:
	$(SCONS) -c

