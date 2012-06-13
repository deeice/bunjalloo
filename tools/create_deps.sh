#!/bin/sh

tooldir=$(dirname $0)
. $tooldir/shell_functions.sh
cd $tooldir
tooldir=$(pwd)

check_devkitpro

upload="no"
VERSION=$(grep -i version $tooldir/../bunjalloo/arm9/version_number.c  | sed 's/.*"\(.*\)".*/\1/g')
TEMP=$(getopt -o huv: --long version:,upload,help -- "$@")
if [ $? != 0 ] ; then
  echo "Try '$0 --help' for more information"
  exit 1
fi

eval set -- "$TEMP"

while true ; do
  case $1 in
    -v|--ve|--ver|--vers|--versi|--versio|--version ) VERSION=$2 ; shift 2 ;;
    -u|--up|--upl|--uplo|--uploa|--upload ) upload="yes" ; shift ;;
    -h|--help )
    echo "Create and optionally upload dependencies tarball."
    echo ""
    echo "Usage: $(basename $0) [OPTION]... "
    echo "Options available are:"
    echo "-v, --version=VERSION    Set this version number"
    echo "-u, --upload             Upload the files too"
    echo "-h, --help               This message."
    exit 0
    ;;
    --) shift ;  break ;;
    *) echo "Internal error! " >&2 ; exit 1 ;;
  esac
done

# copy the files
filenames="
lib/libgif.a
lib/libjpeg.a
lib/libmatrixsslstatic.a
lib/libpng.a
lib/libunzip.a
lib/libz.a
include/gif_lib.h
include/ioapi.h
include/jconfig.h
include/jerror.h
include/jmorecfg.h
include/jpeglib.h
include/matrixCommon.h
include/matrixConfig.h
include/matrixSsl.h
include/pngconf.h
include/png.h
include/unzip.h
include/zconf.h
include/zlib.h
"

mkdir .create_deps
cd .create_deps
mkdir -p lib include
for i in $filenames
do
  cp -v $DEVKITPRO/libnds/$i $i
done
cat > install.sh << EOF
#!/bin/sh

if test -z "\$DEVKITPRO" ; then
  echo "No DEVKITPRO variable set. Set the path to devkit pro."
  exit 1
fi
die() {
  echo \$@ 2>&1
  exit 1
}
for i in lib include
do
  dir=\$DEVKITPRO/libnds/\$i
  mkdir -p \$dir || die "Error creating directory \$dir"
  for file in \$i/*
  do
    cp -v \$file \$DEVKITPRO/libnds/\$i
  done
done
EOF
deptar=bunjalloo-deps-${VERSION}.tar.gz

chmod +x install.sh
tar czvf ../$deptar install.sh lib include
cd ..
rm -rf .create_deps

if [ "$upload" = "yes" ] ; then
  upload_file quirkysoft "Precompiled dependencies for bunjalloo release $VERSION" "Type-Archive,OpSys-NDS" ${deptar}
fi
