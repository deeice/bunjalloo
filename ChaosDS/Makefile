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

distdir=$(target)-dist
dist: all
	mkdir $(distdir) -p
	cp $(target).nds $(distdir)/$(target).nds
	cp COPYING $(distdir)
	zip -r $(target).zip $(distdir)/

