#!/bin/bash

while read line
do
  cd ../.. >/dev/null
  ./bunjalloo $line &
  sleep 5
  pkill bunjalloo
  cd - >/dev/null
done < input.txt

