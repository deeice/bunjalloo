#!/bin/sh

prog=$1
count=$2

let i=0
while [[ $i -lt $count ]] ; do
  /usr/bin/time -f %E $prog > /dev/null
  let i=$i+1
done

