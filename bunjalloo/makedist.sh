#!/bin/bash
SVN="svn"
UPLOAD="googlecode_upload.py"

repo=https://quirkysoft.googlecode.com/svn
trunk=$repo/trunk
tags=$repo/tags
project=bunjalloo
upload="no"
tag="no"
VERSION="0.5"

die() {
  echo >&2 "$@"
  exit 1
}

TEMP=`getopt -o hutv: --long version:,upload,tag,help -- "$@"`
    
if [ $? != 0 ] ; then
  echo "Try '$0 --help' for more information"
  exit 1
fi  

eval set -- "$TEMP"

while true ; do
  case $1 in
    -v|--ve|--ver|--vers|--versi|--versio|--version ) VERSION=$2 ; shift 2 ;;
    -u|--up|--upl|--uplo|--uploa|--upload ) upload="yes" ; shift ;;
    -t|--ta|--tag ) tag="yes" ; shift ;;
    -h|--help )
    echo "Create and optionally upload binary and source distribution files."
    echo ""
    echo "Usage: $(basename $0) [OPTION]... "
    echo "Options available are:"
    echo "-v, --version=VERSION    Set the distro version number"
    echo "-u, --upload             Upload the files too"
    echo "-t, --tag                Create a svn tag for the release"
    echo "-h, --help               This message."
    exit 0
    ;;
    --) shift ;  break ;;
    *) echo "Internal error! " >&2 ; exit 1 ;;
  esac
done

makedistdir=$(dirname $0)
cd $makedistdir
makedistdir=$(pwd)
revision=$(git-svn find-rev HEAD)
# don't include my test cache in the distro!
rm -rf data/bunjalloo/cache
rm -rf data/bunjalloo/user/bookmarks.html
scons -Q dist version=$VERSION || die "Failed to build dist"
zipname=$project-$VERSION.zip
echo "Created $zipname"
src=$project-src-$VERSION
cd ..
git-archive --prefix=$src/ HEAD bunjalloo libndspp | gzip > $src.tar.gz || die "Unable to create $src.tar.gz"
mv $src.tar.gz $makedistdir/ || die "Unable to mv $src.tar.gz to $makedistdir"
cd - > /dev/null
echo "Created $src.tar.gz"

if test "$tag" = "yes" ; then
  tagname=${project}_${VERSION}_r${revision}
  $SVN cp $trunk $tags/$tagname || die "Unable to make tag $tagname"
fi

if test "$upload" = "yes" ; then
  $UPLOAD -s "Source code for $project release $VERSION" -p quirkysoft \
     -l Type-Source,Program-Bunjalloo $src.tar.gz \
    || die "Unable to upload $src.tar.gz"

  $UPLOAD -s "$project release $VERSION" -p quirkysoft \
   -l Type-Archive,Program-Bunjalloo,OpSys-NDS ${zipname} \
    || die "Unable to upload ${zipname}"
fi

