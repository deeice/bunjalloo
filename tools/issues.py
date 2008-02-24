#!/usr/bin/env python

# read in issues page and parse into issue no and title info
#  curl http://code.google.com/p/<site>/issues/list | ./issues.py > issues.csv
#
import sys
import re

issues = {}
header = []

while True:
  line = sys.stdin.readline()
  if not line:
    break
  m = re.match('.*detail\?id=([0-9]+)">([^<]+)<.*', line)
  h = re.match('^ ><a href="#"[^>]+>([^<]+)<.*', line)
  if m:
    g = m.group
    if (issues.has_key(g(1))):
      issues[g(1)].append(g(2))
    else:
      issues[g(1)] = [g(2)]
  elif h:
    header.append(h.group(1))

print ','.join(header)
for k in issues.keys():
  print ','.join(issues[k])
