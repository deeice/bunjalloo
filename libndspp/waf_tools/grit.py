""" Waf tool for generating image data using grit """
import os, os.path
import Params, Action

def output_for_source(source, env):
  """ Given a file name generate a list of outputs """
  # add img.bin, pal.bin and map.bin to the output as side effects
  if ('-fts' in env['GRITFLAGS']):
    fileName = source.split(os.path.extsep)[0]
    out = []
    out.append('%s.s' % (fileName))
    out.append('%s.h' % (fileName))
  elif ('-ftb' in env['GRITFLAGS']):
    fileName = source.split(os.path.extsep)[0]
    out = []
    out.append('%s.img.bin' % (fileName))
    out.append('%s.pal.bin' % (fileName))
  return out

from Object import extension, taskgen

def add_include(task):
  """ Add an #include line to the output file for the given input file  """
  outfile = task.m_outputs[0].abspath(task.env())
  infile = task.m_inputs[0].abspath(task.env())
  fp = open(outfile, 'w')
  fp.write('#include "%s"\n' % (os.path.basename(infile).replace('.c','.h')))
  i = open(infile)
  for k in i.readlines():
    fp.write(k)
  fp.close()


@taskgen
@extension('.png')
def img_file(self, node):
  """ Convert a png file to a cpp and header file """
  out_source_s = node.change_ext('.c')
  out_source_C = node.change_ext('.cpp')
  out_source_h = node.change_ext('.h')

  tsk = self.create_task('grit')
  tsk.set_inputs(node)
  tsk.set_outputs([out_source_s, out_source_h])

  tsk = self.create_task('c_C')
  tsk.set_inputs(out_source_s)
  tsk.set_outputs(out_source_C)

  # the out file is to be processed as a cpp file
  self.allnodes.append(out_source_C)

def detect(conf):
  dka_bin = '%s/bin' % (Params.g_options.devkitarm)
  grit = 'grit'
  grit = conf.find_program(grit, path_list=[dka_bin], var='GRIT')
  if not grit:
    conf.fatal('grit was not found')
  conf.env['GRIT'] = grit
  if not conf.env['GRITFLAGS']:
    conf.env['GRITFLAGS'] = '-pw 16 -gB 8 -m! -ft b -fh! -q'.split()

def setup(bld):
  grit_str = '${GRIT} ${SRC[0].abspath(env)} -o ${SRC[0].bldbase(env)} ${GRITFLAGS} > /dev/null 2>&1 '
  Action.simple_action('grit', grit_str, color='CYAN', prio=10)
  Action.Action('c_C', vars=[], func=add_include , color='CYAN', prio=11)

