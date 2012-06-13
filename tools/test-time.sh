#!/bin/bash

prog=$1
count=$2

let i=0
while [[ $i -lt $count ]] ; do
  /usr/bin/time -f %e $prog > /dev/null
  let i=$i+1
done 2>&1 | tee /tmp/times$$.log

python <<EOF
total = 0
times = 0
for line in open("/tmp/times$$.log"):
    times += 1
    total += float(line)
print "Avg: " + str(total/times)
EOF

rm /tmp/times$$.log
