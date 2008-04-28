import os, os.path
import Params, Action, Object, misc

def dldi_func(tsk):
  misc.copy_func(tsk)
  env = tsk.env()
  infile = tsk.m_inputs[0].abspath(env)
  Action.g_actions['dlditool'].run(tsk)


class dlditool_taskgen(Object.task_gen):
  def __init__(self, *k, **kw):
    Object.task_gen.__init__(self, *k, **kw)

  def apply(self):
    find_build = self.path.find_build
    input_nodes = []
    for filename in self.to_list(self.source):
      node = find_build(filename)
      outnode = node
      input_nodes.append(node)
    # niceness of 160
    task = self.create_task('copy', self.env, 160)
    task.chmod = ''
    task.fun = dldi_func
    task.set_inputs(input_nodes)
    task.set_outputs(find_build(self.target))

def detect(conf):
  dka_bin='%s/bin'%Params.g_options.devkitarm
  dlditool='dlditool'
  dlditool = conf.find_program(dlditool, path_list=[dka_bin], var='DLDITOOL')
  if not dlditool:
    conf.fatal('dlditool was not found')
  conf.env['DLDITOOL'] = dlditool

def setup(bld):
  dlditool_str = '${DLDITOOL} ${DLDIFLAGS} ${TGT} > /dev/null'
  Action.simple_action('dlditool', dlditool_str, color='BLUE', prio=160)
