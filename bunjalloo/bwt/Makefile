export LIBNDSPP :=      $(CURDIR)/../../libndspp

all:
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -C common -f Makefile clean
	$(MAKE) -C common -f Makefile.sdl clean
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

