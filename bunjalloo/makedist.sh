#!/bin/bash

project=bunjalloo
upload="no"
tag="no"
makedistdir=$(dirname $0)
. ${makedistdir}/../tools/shell_functions.sh
pushd $makedistdir > /dev/null

if test "x$WAFDIR" = x ; then
  WAF_SCRIPT=$(which waf)
  WAF=$WAF_SCRIPT
else
  WAF=$WAFDIR/../waf-light
  WAF_SCRIPT=$WAFDIR/../waf
  if ! test -e $WAF_SCRIPT ; then
    cd $WAFDIR/..
    ./waf-light --make-waf || die "Unable to build suitable waf script"
    cd -
  fi
fi
VERSION=$(grep -i version arm9/version_number.c  | sed 's/.*"\(.*\)".*/\1/g')


TEMP=$(getopt -o hutv:l: --long version:,last:,upload,tag,help -- "$@")

if [ $? != 0 ] ; then
  echo "Try '$0 --help' for more information"
  exit 1
fi

last=$(hg tags | awk '!/^tip/ {print $1; exit}')

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
    echo "-t, --tag                Create a tag for the release"
    echo "-h, --help               This message."
    echo "-l, --last=LAST          Use LAST as the branch/tag for the last release."
    exit 0
    ;;
    --) shift ;  break ;;
    *) echo "Internal error! " >&2 ; exit 1 ;;
  esac
done

makedistdir=$(pwd)

# Create the zip file
distdir=$project-$VERSION
zipname=$distdir.zip
$WAF check -p || die "Error in build"
cd ..
$WAF configure --prefix=$distdir || die "Error running configure"
cd -
$WAF install > /dev/null || die "Error in install"
pushd ..
pushd $distdir > /dev/null || die "Unable to cd to $distdir"
mv $project.nds $project-${VERSION//./}.nds || die "Error creating NDS file"
zip -r ../$zipname . > /dev/null || die "Unable to create $zipname"
popd > /dev/null
mv $zipname $makedistdir
rm -rf $distdir
popd > /dev/null
echo "Created $zipname"

# Create the tar.gz source code file
src=$project-$VERSION
src_tarname=$project-src-$VERSION.tar
src_tgzname=$project-src-$VERSION.tar.gz
pushd .. >/dev/null
hg archive --prefix=$src/ -r tip --type=tar $src_tarname || die "Unable to create $src_tarname"
mkdir $src -p
tar xf $src_tarname && rm $src_tarname
cp $WAF_SCRIPT $src
tar czf $src_tgzname $src && rm -rf $src
mv $src_tgzname $makedistdir/ || die "Unable to mv $src_tgzname to $makedistdir"
echo "Created $src_tgzname"

hg log -M --template="  * {desc|firstline|strip}\n" -r tip:$last bunjalloo libndspp \
                > $makedistdir/ShortLog-$VERSION || die "Unable to create ShortLog"
hg log -M --style changelog -r tip:$last bunjalloo libndspp \
                > $makedistdir/ChangeLog-$VERSION || die "Unable to create ChangeLog"
echo "Created ChangeLog-$VERSION and ShortLog-$VERSION"
popd > /dev/null

if test "$tag" = "yes" ; then
  hg tag -m "Bunjalloo release ${VERSION}" v${VERSION}
fi

if test "$upload" = "yes" ; then
  upload_file quirkysoft "Source code for $project release $VERSION" "Type-Source,Program-Bunjalloo" $src_tgzname
  upload_file quirkysoft "$project release $VERSION" "Type-Archive,Program-Bunjalloo,OpSys-NDS" ${zipname}
fi
