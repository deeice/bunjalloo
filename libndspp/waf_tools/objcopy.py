import os, sys
import optparse
import Params, Configure,Action, Utils
import Object

class objcopyobj(Object.task_gen):
  def __init__(self):
    Object.task_gen.__init__(self)
  def apply(self):
    find_source_lst = self.path.find_source_lst
    input_nodes = []
    for filename in self.to_list(self.source):
      node = find_source_lst(Utils.split_path(filename))
      task = self.create_task('objcopy', self.env)
      task.set_inputs(node)
      task.set_outputs(node.change_ext('.bin'))

def detect(conf):
  v = conf.env
  dka_bin='%s/bin'%os.environ['DEVKITARM']
  arm_eabi='arm-eabi-objcopy'
  objcopy = conf.find_program(arm_eabi, path_list=[dka_bin], var='OBJCOPY')
  if not objcopy:
    conf.fatal('objcopy was not found')
  v['OBJCOPY'] = objcopy

def setup(bld):
  objcopy_str = '${OBJCOPY} -O binary ${SRC} ${TGT}'
  Action.simple_action('objcopy', objcopy_str, color='BLUE', prio=130)

