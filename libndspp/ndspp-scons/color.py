import os.path, sys

black = '\x1b[0m'
green = '\x1b[30;32m'
blue = '\x1b[30;34m'

def generate(env, **kw):
  """ Generate a coloured output environment """
  def print_cmd_line(s, targets, sources, env):
    # print s
    sourceTxt = ','.join( [ os.path.basename(str(f)) for f in sources] )
    if len(sourceTxt) > 80:
      sourceTxt = sourceTxt[0:75]+'%s...'%black
    sourceTxt = sourceTxt.replace(',', '%s,%s'%(black, blue))
    sys.stdout.write('\t%s%s%s -> %s%s%s...\n'%
        (blue, sourceTxt, black,
         green,('%s,%s'%(black, green)).join( [ os.path.basename(str(f)) for f in targets] ), black ))

  env.Append(PRINT_CMD_LINE_FUNC=print_cmd_line)

def exists(env):
  return 1
