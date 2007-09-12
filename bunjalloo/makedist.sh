#!/bin/bash
SVN="svn"
MV="mv"
UPLOAD="googlecode_upload.py"

trunk=https://quirkysoft.googlecode.com/svn/trunk
tags=https://quirkysoft.googlecode.com/svn/tags
project=bunjalloo

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
$SVN cp $trunk $tags/${project}_${release}_r${revision}
