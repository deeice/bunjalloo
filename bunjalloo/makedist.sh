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

TEMP=`getopt -o hutv:l: --long version:,last:,upload,tag,help -- "$@"`
    
if [ $? != 0 ] ; then
  echo "Try '$0 --help' for more information"
  exit 1
fi  

last=$(git branch -r | grep $project | grep -v tags|tail -1|cut -c3-)

eval set -- "$TEMP"

while true ; do
  case $1 in
    -v|--ve|--ver|--vers|--versi|--versio|--version ) VERSION=$2 ; shift 2 ;;
    -u|--up|--upl|--uplo|--uploa|--upload ) upload="yes" ; shift ;;
    -t|--ta|--tag ) tag="yes" ; shift ;;
    -l|--la|--las|--last ) last=$2 ; shift 2 ;;
    -h|--help )
    echo "Create and optionally upload binary and source distribution files."
    echo ""
    echo "Usage: $(basename $0) [OPTION]... "
    echo "Options available are:"
    echo "-v, --version=VERSION    Set the distro version number"
    echo "-u, --upload             Upload the files too"
    echo "-t, --tag                Create a svn tag for the release"
    echo "-h, --help               This message."
    echo "-l, --last=LAST          Use LAST as the branch/tag for the last release."
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
# don't include my test files in the distro!
rm -rf data/bunjalloo/cache
rm -rf data/bunjalloo/user/*
rm -f data/bunjalloo/config.ini

scons -Q dist version=$VERSION || die "Failed to build dist"
zipname=$project-$VERSION.zip
echo "Created $zipname"
src=$project-src-$VERSION
cd ..
git-archive --prefix=$src/ HEAD bunjalloo libndspp | gzip > $src.tar.gz || die "Unable to create $src.tar.gz"
mv $src.tar.gz $makedistdir/ || die "Unable to mv $src.tar.gz to $makedistdir"
cd - > /dev/null
echo "Created $src.tar.gz"

git log --pretty=format:"  * %s" --no-merges HEAD ^$last \
                > ChangeLog-$VERSION || die "Unable to create ChangeLog"
echo "Created ChangeLog-$VERSION"


if test "$tag" = "yes" ; then
  tagname=${project}_${VERSION}_r${revision}
  $SVN cp $trunk $tags/$tagname || die "Unable to make tag $tagname"
fi

if test "$upload" = "yes" ; then
  authfile=$(grep $(dirname $repo) $HOME/.subversion/auth/svn.simple/* -l)
  user=$(grep username -2 $authfile | tail -1)
  pass=$(grep password -2 $authfile | tail -1)
  $UPLOAD -s "Source code for $project release $VERSION" --project=quirkysoft --user=$user --password=$pass \
     -l Type-Source,Program-Bunjalloo $src.tar.gz \
    || die "Unable to upload $src.tar.gz"

  $UPLOAD -s "$project release $VERSION" --project=quirkysoft --user=$user --password=$pass \
   -l Type-Archive,Program-Bunjalloo,OpSys-NDS ${zipname} \
    || die "Unable to upload ${zipname}"
fi

