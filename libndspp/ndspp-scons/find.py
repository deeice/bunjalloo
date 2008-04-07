#!/usr/bin/env python
import os

def find(top='.', includes=[], excludes=[]):
  result = []
  for (root, dirs, files) in os.walk(top):
    exclude = [ e for e in excludes if root.find(e) != -1]
    if not exclude:
      for child in files:
        exclude = [ e for e in excludes if child.find(e) != -1]
        if not exclude and ((not includes) or (includes and os.path.splitext(child)[1] in includes)):
          result.append( os.path.join(root, child))
  return result


