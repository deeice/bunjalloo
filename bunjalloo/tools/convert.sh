#!/bin/bash

if [[ $# -lt 1 ]] ; then
  echo Usage $0 ttf [size]
  exit 1
fi
inFile=$1
# 11 is the default size
size=11
if [[ $# -gt 1 ]] ; then
  size=$2
  shift ;
fi
shift ;

fontName=$(basename $inFile)
python ttf2png.py $inFile $size $* \
&& convert $fontName.png -colors 16 $fontName.bmp && grit $fontName.bmp -ft b -fh\! -gb -gB 4 -gT FFFFFF

goodName=$(basename $inFile .ttf | tr '[:upper:]' '[:lower:]')

mv $fontName.png $goodName.png
mv $fontName.pal.bin $goodName.pal
mv $fontName.img.bin $goodName.img
mv $fontName.map $goodName.map


