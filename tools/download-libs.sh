#!/bin/sh

LIBPNG=libpng-1.2.22
ZLIB=zlib-1.2.3

if [ ! -e ${LIBPNG}.tar.bz2 ] ; then wget ftp://ftp.simplesystems.org/pub/libpng/png/src/${LIBPNG}.tar.bz2 ; fi
if [ ! -e ${ZLIB}.tar.gz ] ; then wget http://www.zlib.net/${ZLIB}.tar.gz ; fi
tar xjvf ${LIBPNG}.tar.bz2
tar xzvf ${ZLIB}.tar.gz
ln -s ${ZLIB} zlib
ln -s ${LIBPNG} libpng
wget http://quirkysoft.googlecode.com/svn/external/zlib/Makefile.ds -O zlib/Makefile.ds
wget http://quirkysoft.googlecode.com/svn/external/libpng/Makefile.ds -O libpng/Makefile.ds
make -C zlib -f Makefile.ds
make -C libpng -f Makefile.ds
