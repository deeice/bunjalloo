.PHONY: all clean docs checkdupes

target=$(shell basename $(CURDIR))

all: 
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg


checkdupes:
	java -jar /home/rich/bin/checkstyle-all-4.3.jar -c /home/rich/bin/dupe.xml arm9/source/*.* 

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

