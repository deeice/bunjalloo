.PHONY: all clean docs arm9/source/version.c

target=$(shell basename $(CURDIR))
export DATADIR:="\"data/$(target)\""

export DATESTRING:="\"$(shell date '+%Y%m%d')\""

all:  ctags arm9/source/version.c
	$(MAKE) -C bwt
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -C common clean
	$(MAKE) -C common -f Makefile.sdl clean
	$(MAKE) -C bwt clean
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . ../libndspp

distdir=$(target)-dist

$(distdir):
	mkdir $(distdir) -p

dist: all $(distdir)
	cp $(target)-unpatched.nds $(distdir)/$(target).nds
	cp data/ -rvfp $(distdir)/
	find $(distdir) -name '.svn' -type d | xargs rm -rf 
	cd $(distdir) && zip -r $(target).zip *
	mv $(distdir)/$(target).zip .
	rm -rf $(distdir)

dist-src: $(distdir)
	cd $(distdir) && svn export $(REP)/bunjalloo
	cd $(distdir) && svn export $(REP)/libndspp
	tar czvf $(distdir).tar.gz $(distdir)
