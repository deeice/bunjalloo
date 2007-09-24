#!/bin/bash
SVN="svn"
MV="mv"
UPLOAD="googlecode_upload.py"

trunk=https://quirkysoft.googlecode.com/svn/trunk
tags=https://quirkysoft.googlecode.com/svn/tags
project=bunjalloo

$SVN up
modifications=$($SVN status -q | wc -l)
if [ $modifications != 0 ] ; then
  echo "Modified files exist.. commit first before tagging stuff!"
  exit 1
fi

read -p "Version? " release
read -p "Summary? " summary

revision=$(svn info | grep ^Revision | cut -d: -f2 | tr -d ' ')

make dist
$MV ${project}.zip ${project}-${release}.zip
$UPLOAD -s "$summary" -p quirkysoft -l Type-Archive,Program-Bunjalloo,OpSys-NDS ${project}-${release}.zip
tagname=${project}_${release}_r${revision}
$SVN cp $trunk $tags/$tagname

REP=${tags/https/http}/$tagname
distdir=${project}-${release}
mkdir -p $distdir
cd $distdir
svn export ${REP}/${project}
svn export ${REP}/libndspp
cd ..
tar czvf ${distdir}.tar.gz ${distdir}

$MV ${project}-${release}.tar.gz ${project}-src-${release}.tar.gz
$UPLOAD -s "Source code for release $release" -p quirkysoft -l Type-Source,Program-Bunjalloo ${project}-src-${release}.tar.gz
