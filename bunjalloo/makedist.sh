#!/bin/bash
SVN="svn"
UPLOAD="googlecode_upload.py"

repo=https://quirkysoft.googlecode.com/svn
trunk=$repo/trunk
tags=$repo/tags
project=bunjalloo
upload="no"
tag="no"
WAF_SCRIPT=$(which waf)
VERSION=$( grep -i version arm9/version.c  | sed 's/.*"\(.*\)".*/\1/g')

die() {
  echo >&2 "$@"
  exit 1
}

TEMP=$(getopt -o hutv:l: --long version:,last:,upload,tag,help -- "$@")
    
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
pushd $makedistdir > /dev/null
makedistdir=$(pwd)
revision=$(git-svn find-rev HEAD)

# Create the zip file
distdir=$project-$VERSION
zipname=$distdir.zip
waf --install-to=$distdir install > /dev/null || die "Error in build"
pushd $distdir > /dev/null || die "Unable to cd to $distdir"
zip -r ../$zipname * > /dev/null || die "Unable to create $zipname"
popd > /dev/null
rm -rf $distdir
echo "Created $zipname"

# Create the tar.gz source code file
src=$project-src-$VERSION
src_tarname=$src.tar
src_tgzname=$src.tar.gz
pushd .. >/dev/null
git-archive --prefix=$src/ HEAD bunjalloo libndspp > $src_tarname || die "Unable to create $src_tarname"
mkdir $src -p
tar xf $src_tarname && rm $src_tarname
for i in $src/* ; do
  cp $WAF_SCRIPT $i
done
tar czf $src_tgzname $src && rm -rf $src
mv $src_tgzname $makedistdir/ || die "Unable to mv $src_tgzname to $makedistdir"
echo "Created $src_tgzname"

git log --pretty=format:"  * %s" --no-merges HEAD ^$last -- bunjalloo libndspp \
                > $makedistdir/ShortLog-$VERSION || die "Unable to create ChangeLog"
git log --no-merges HEAD ^$last -- bunjalloo libndspp \
                > $makedistdir/ChangeLog-$VERSION || die "Unable to create ChangeLog"
echo "Created ChangeLog-$VERSION and ShortLog-$VERSION"
popd > /dev/null

if test "$tag" = "yes" ; then
  tagname=${project}_${VERSION}_r${revision}
  $SVN cp $trunk $tags/$tagname || die "Unable to make tag $tagname"
fi

if test "$upload" = "yes" ; then
  authfile=$(grep $(dirname $repo) $HOME/.subversion/auth/svn.simple/* -l)
  user=$(grep username -2 $authfile | tail -1)
  pass=$(grep password -2 $authfile | tail -1)
  $UPLOAD -s "Source code for $project release $VERSION" --project=quirkysoft --user=$user --password=$pass \
     -l Type-Source,Program-Bunjalloo $src_tgzname \
    || die "Unable to upload $src_tgzname"

  $UPLOAD -s "$project release $VERSION" --project=quirkysoft --user=$user --password=$pass \
   -l Type-Archive,Program-Bunjalloo,OpSys-NDS ${zipname} \
    || die "Unable to upload ${zipname}"
fi
