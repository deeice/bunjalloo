import os, os.path
import Params, Configure,Action, Object, Utils

def output_for_source(source, env):
  # add img.bin, pal.bin and map.bin to the output as side effects
  if ('-fts' in env['GRITFLAGS']):
    fileName = source.split(os.path.extsep)[0]
    t = []
    t.append('%s.s'%(fileName))
    t.append('%s.h'%(fileName))
  elif ('-ftb' in env['GRITFLAGS']):
    fileName = source.split(os.path.extsep)[0]
    t = []
    t.append('%s.img.bin'%(fileName))
    t.append('%s.pal.bin'%(fileName))
  # t.append('%s.map.bin'%(fileName))
  return t
# class gritobj(Object.task_gen):
#   s_default_ext = ['.png', '.bmp']
#   def __init__(self):
#     Object.task_gen.__init__(self)
#   def output_for_source(self, source):
#     # add img.bin, pal.bin and map.bin to the output as side effects
#     if ('-fts' in self.env['GRITFLAGS']):
#       fileName = source.split(os.path.extsep)[0]
#       t = []
#       t.append('%s.s'%(fileName))
#       t.append('%s.h'%(fileName))
#     elif ('-ftb' in self.env['GRITFLAGS']):
#       fileName = source.split(os.path.extsep)[0]
#       t = []
#       t.append('%s.img.bin'%(fileName))
#       t.append('%s.pal.bin'%(fileName))
#     # t.append('%s.map.bin'%(fileName))
#     return t

#   def apply(self):
#     find_source = self.path.find_source
#     find_build = self.path.find_build
#     for filename in self.to_list(self.source):
#       node = find_source(filename)
#       output_nodes = []
#       for bld in self.output_for_source(filename):
#         output_nodes.append(find_build(bld))
#       task = self.create_task('grit', self.env)
#       task.set_inputs(node)
#       task.set_outputs(output_nodes)
#       self.allnodes.extend(output_nodes)

from Object import extension, taskgen

@taskgen
@extension('.png')
def coin_file(self, node):
  out_source_s = node.change_ext('.c')
  out_source_h = node.change_ext('.h')


  tsk = self.create_task('grit')
  tsk.set_inputs(node)
  tsk.set_outputs([out_source_s, out_source_h])

  # the out file is to be processed as a cpp file
  self.allnodes.append(out_source_s)


def detect(conf):
  dka_bin='%s/bin'%os.environ['DEVKITARM']
  grit='grit'
  grit = conf.find_program(grit, path_list=[dka_bin], var='GRIT')
  if not grit:
    conf.fatal('grit was not found')
  conf.env['GRIT'] = grit
  conf.env['GRITFLAGS'] = '-pw 16 -gB 8 -m! -ft b -fh! -q'.split()

def setup(bld):
  grit_str = '${GRIT} ${SRC[0].abspath(env)} -o ${SRC[0].bldbase(env)} ${GRITFLAGS} > /dev/null 2>&1 '
  Action.simple_action('grit', grit_str, color='CYAN', prio=10)

