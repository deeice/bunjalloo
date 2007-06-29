.PHONY: all clean docs

target=$(shell basename $(CURDIR))

all: 
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg

version := 2.0

distdir=$(target)-$(version)
dist: all
	mkdir $(distdir) -p
	cp $(target).nds $(distdir)/$(target).nds
	cp $(target).ds.gba $(distdir)/$(target).ds.gba
	cp README $(distdir)
	cp COPYING $(distdir)
	zip -r $(target)-$(version).zip $(distdir)/
	rm -rf $(distdir)

