#!/bin/sh

LIBPNG=libpng-1.2.22
ZLIB=zlib-1.2.3
GIFLIB=giflib-4.1.4

die() {
  echo >&2 "$@"
  exit 1
}

if [ ! -e ${LIBPNG}.tar.bz2 ] ; then
  wget ftp://ftp.simplesystems.org/pub/libpng/png/src/${LIBPNG}.tar.bz2 || \
  die "Unable to download ${LIBPNG}"
fi
if [ ! -e ${ZLIB}.tar.gz ] ; then
  wget http://www.zlib.net/${ZLIB}.tar.gz || \
  die "Unable to download ${ZLIB}"
fi
if [ ! -e ${GIFLIB}.tar.gz ] ; then
  wget http://downloads.sourceforge.net/libungif/${GIFLIB}.tar.gz  || \
  die "Unable to download ${GIFLIB}"
fi

echo "Extracting downloaded libraries..."
tar xjf ${LIBPNG}.tar.bz2 || die "Problems unpacking ${LIBPNG}"
tar xzf ${ZLIB}.tar.gz    || die "Problems unpacking ${ZLIB}"
tar xzf ${GIFLIB}.tar.gz  || die "Problems unpacking ${GIFLIB}"

ln -sf ${ZLIB} zlib
ln -sf ${LIBPNG} libpng
ln -sf ${GIFLIB} giflib

echo "Checking out more libraries..."
svn co http://quirkysoft.googlecode.com/svn/external/ otherlibs || die "Unable to checkout from subversion"
cp otherlibs/zlib/Makefile.ds       zlib/Makefile.ds
cp otherlibs/libpng/Makefile.ds     libpng/Makefile.ds
cp otherlibs/giflib/Makefile.ds     giflib/Makefile.ds
cp otherlibs/giflib/config.h        giflib/config.h
cp otherlibs/giflib/lib/Makefile.ds giflib/lib/Makefile.ds

echo "Building and installing the libraries..."
make -C zlib -f Makefile.ds install || die "Problems building zlib"
make -C libpng -f Makefile.ds install || die "Problems building libpng"
make -C giflib -f Makefile.ds install || die "Problems building giflib"
make -C otherlibs/jpgd -f Makefile.ds install || die "Problems building jpgd"
make -C otherlibs/matrixssl/src -f Makefile.ds install || die "Problems building matrixssl"

echo
echo
echo "You should also run the following to uninstall the dependencies:"
echo "  make -C zlib -f Makefile.ds uninstall"
echo "  make -C libpng -f Makefile.ds uninstall"
echo "  make -C giflib -f Makefile.ds uninstall"
echo "  make -C otherlibs/jpgd -f Makefile.ds uninstall"
echo "  make -C otherlibs/matrixssl/src -f Makefile.ds uninstall"
