#!/bin/sh

cd _build_
for i in $(find default -name '*.gcda')
do
    d=$(dirname $i)
    gcov -p $i -o $d
done
