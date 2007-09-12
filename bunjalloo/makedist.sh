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
$MV bunjalloo.zip bunjalloo-${release}.zip
$UPLOAD -s "$summary" -p quirkysoft -l Type-Archive,Program-Bunjalloo,OpSys-NDS bunjalloo-${release}.zip
tagname=${project}_${release}_r${revision}
$SVN cp $trunk $tags/$tagname

make dist-src REP=${tags/https/http}/$tagname
$MV bunjalloo-dist.tar.gz bunjalloo-src-${release}.tar.gz
$UPLOAD -s "Source code for release $release" -p quirkysoft -l Type-Source,Program-Bunjalloo bunjalloo-src-${release}.tar.gz
