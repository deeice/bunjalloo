import os
import stat

def walktree (top = "."):
  names = os.listdir(top)
  yield top, names
  for name in names:
    try:
      st = os.lstat(os.path.join(top, name))
    except os.error:
      continue
    if stat.S_ISDIR(st.st_mode):
      for (newtop, children) in walktree (os.path.join(top, name)):
        yield newtop, children

def find(top='.', includes=[], excludes=[]):
  files = []
  for (basepath, children) in walktree(top):
      exclude = [ e for e in excludes if basepath.find(e) != -1]
      if not exclude:
        for child in children:
            exclude = [ e for e in excludes if child.find(e) != -1]
            if not exclude and ((not includes) or (includes and os.path.splitext(child)[1] in includes)):
                files.append( os.path.join(basepath, child))
  return files

if __name__ == '__main__':
  includes = ['.cpp', '.c','.h','.inl']
  exclude = ['/build/']
  for i in find('.', includes, exclude):
    print i

