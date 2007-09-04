.PHONY: all clean docs arm9/source/version.c

target=$(shell basename $(CURDIR))

export LIBBWT:=$(CURDIR)/../../bwt
export LIBNDSPP:=$(CURDIR)/../../../libndspp

all:
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg

