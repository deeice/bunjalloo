.PHONY: all clean docs

target=$(shell basename $(CURDIR))
export DATADIR:="\"data/$(target)\""

all:
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg

distdir=$(target)-dist
dist: all
	mkdir $(distdir) -p
	cp $(target)-unpatched.nds $(distdir)/$(target).nds
	cp COPYING data/$(target)/docs/copying
	cp data/ -rvfp $(distdir)/
	zip -r $(target).zip $(distdir)/

