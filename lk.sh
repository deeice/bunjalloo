#!/bin/sh

echo "making bunjalloo"
cd bunjalloo
./mk.sh
cd ..

cp bunjalloo/bunji bin
