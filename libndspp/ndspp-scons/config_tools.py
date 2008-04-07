#!/usr/bin/env python
import sys, os
def check_platform():
  if not sys.platform.startswith('linux'):
    print "Sorry, only compilation on Linux is supported."
    sys.exit(1)

def run_configure():
  result = os.system('which scons > /dev/null 2>&1')
  if os.WEXITSTATUS(result):
    print """
This program uses the scons software construction tool. See www.scons.org for
more details. You may be able to install it by running something like:
    su -
    yast --install scons
or
    sudo apt-get install scons
"""
    sys.exit(result)

  result = os.system('scons configure')
  if os.WEXITSTATUS(result) != 0:
    sys.exit(result)


def create_makefiles(here):
  Makefile = """
# Makefile automatically generated.
SCONS := $(shell which scons)
SCONSOPTS := -Q%s
all:
	@$(SCONS) $(SCONSOPTS)

clean:
	@$(SCONS) $(SCONSOPTS) -c

distclean:
	-rm -rf build .scon*
	-find . -name Makefile -exec rm -f '{}' \;
"""

  from find import find
  for f in find(here):
    try:
      if f.endswith('SConscript'):
        makefile = os.path.join( os.path.dirname(f), 'Makefile')
        print "creating %s"%makefile
        fp = open(makefile, 'w')
        fp.write(Makefile%(' -u'))
      if f.endswith('SConstruct'):
        makefile = os.path.join( os.path.dirname(f), 'Makefile')
        print "creating %s"%makefile
        fp = open(makefile, 'w')
        fp.write(Makefile%(''))
    except Exception, e:
      print 'Error:'+str(e)
      sys.exit(2)

