import os, os.path
import Params, Configure,Action, Object, Utils

class ndstool_taskgen(Object.task_gen):
  def __init__(self, *k, **kw):
    Object.task_gen.__init__(self, *k, **kw)
    self.banner = ''
    self.icon = ''

  def apply(self):
    find_build = self.path.find_build
    input_nodes = []
    for filename in self.to_list(self.source):
      node = self.path.find_build(filename)
      outnode = node
      input_nodes.append(node)
    taskname = 'ndstool_9'
    if len(input_nodes) > 1:
      taskname = 'ndstool_7_9'
    if self.banner:
      taskname += '_b'
      self.env['NDSTOOL_BANNER'] = self.banner
      node = self.path.find_source(self.icon)
      if not node:
        Params.fatal('ndstool: icon "%s" not found'%self.icon)
      input_nodes.append(node)
    task = self.create_task(taskname, self.env)
    task.set_inputs(input_nodes)
    task.set_outputs(find_build(self.target))

def detect(conf):
  dka_bin='%s/bin'%os.environ['DEVKITARM']
  ndstool='ndstool'
  ndstool = conf.find_program(ndstool, path_list=[dka_bin], var='NDSTOOL')
  if not ndstool:
    conf.fatal('ndstool was not found')
  conf.env['NDSTOOL'] = ndstool

def setup(bld):
  ndstool_str = '${NDSTOOL} -c ${TGT} -7 ${SRC[0].bldpath(env)} -9 ${SRC[1].bldpath(env)} %s > /dev/null'
  Action.simple_action('ndstool_7_9', ndstool_str%'', color='BLUE', prio=150)
  Action.simple_action('ndstool_7_9_b', ndstool_str%'-b ${SRC[2].srcpath(env)} \'${NDSTOOL_BANNER}\'', color='BLUE', prio=150)
  ndstool_str = '${NDSTOOL} -c ${TGT} -9 ${SRC[0].bldpath(env)} %s > /dev/null'
  Action.simple_action('ndstool_9', ndstool_str%'', color='BLUE', prio=150)
  Action.simple_action('ndstool_9_b', ndstool_str%'-b ${SRC[1].srcpath(env)} ${NDSTOOL_ICON} \'${NDSTOOL_BANNER}\'', color='BLUE', prio=150)

