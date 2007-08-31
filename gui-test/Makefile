.PHONY: all clean docs arm9/source/version.c

target=$(shell basename $(CURDIR))
export DATADIR:="\"data/$(target)\""

all:  ctags arm9/source/version.c
	$(MAKE) -f Makefile.ds
	$(MAKE) -f Makefile.sdl

clean:
	$(MAKE) -f Makefile.ds clean
	$(MAKE) -f Makefile.sdl clean

docs:
	@-doxygen doxygen.cfg

ctags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

arm9/source/version.c:
	version=$$(svn info | grep ^Revision | cut -d: -f2 | tr -d ' ') ;\
	currversion=0 ;\
	if [ -e $@ ] ; then \
	  currversion=$$(grep VERSION $@ | cut -d' ' -f6 | tr -d '"') ;\
	fi ;\
	if [ x$$version != x ] ; then \
	  if [ $$currversion -lt $$version ] ; then \
	    echo 'const char * VERSION = "'$$version'" ;' > $@ ;\
	  fi ;\
	fi


distdir=$(target)-dist
dist: all
	mkdir $(distdir) -p
	cp $(target)-unpatched.nds $(distdir)/$(target).nds
	cp data/ -rvfp $(distdir)/
	find $(distdir) -name '.svn' -type d | xargs rm -rf 
	zip -r $(target).zip $(distdir)/

