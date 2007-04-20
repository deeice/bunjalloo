.PHONY: clean all

SUBDIRS:= $(shell ls | egrep -v '^Main')

all:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i ; if [ $$? != 0 ] ; then exit 1 ; fi ; fi; done;

clean:
	rm -f *.o
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i clean ; fi; done;
