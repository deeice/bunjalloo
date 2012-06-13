#!/bin/sh

echo "making libndspp/source/common"
cd libndspp/source/common
./mk.sh
cd ../../..

echo "making libndspp/source/pc"
cd libndspp/source/pc
./mk.sh
cd ../../..

echo "making bunjalloo/arm9"
cd bunjalloo/arm9
./mk.sh
cd ../..

echo "making bunjalloo/bwt/source"
cd bunjalloo/bwt/source
./mk.sh
cd ../../..

echo "making bunjalloo"
cd bunjalloo
./mk.sh
cd ..

cp bunjalloo/bunji bin
