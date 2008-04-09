import os, sys
import optparse
import Params, Configure,Action, Utils, ccroot, misc
import Object

class objcopy_taskgen(Object.task_gen):
  #def __init__(self):
  #  Object.task_gen.__init__(self)
  def apply(self):
    find_source_lst = self.path.find_source_lst
    input_nodes = []
    for filename in self.to_list(self.source):
      node = find_source_lst(Utils.split_path(filename))
      task = self.create_task('objcopy', self.env)
      task.set_inputs(node)
      task.set_outputs(node.change_ext('.bin'))

class bin2o_taskgen(Object.task_gen):
  def __init__(self, *k, **kw):
    Object.task_gen.__init__(self, *k, **kw)
    self.compiled_tasks = []
    self.uselib = ''
    self.uselib_local = ''
    self.m_type = 'staticlib'

  def apply(self):
    find_source_lst = self.path.find_source_lst
    input_nodes = []
    for filename in self.to_list(self.source):
      node = find_source_lst(Utils.split_path(filename))
      # file -> file.bin
      ext = os.path.splitext(filename)[1]+'.bin'
      cpytask = self.create_task('copy', self.env, 10)
      cpytask.chmod = 0
      cpytask.fun = misc.copy_func
      cpytask.set_inputs(node)
      copybin = node.change_ext(ext)
      cpytask.set_outputs(copybin)

      otsk = self.create_task('bin2o', self.env)
      otsk.set_inputs(copybin)
      outnode = copybin.change_ext('.o')
      otsk.set_outputs(outnode)

      self.compiled_tasks.append(otsk)
      input_nodes.append(node)

    if self.target:
      self.link_task = self.create_task('ar_link_static', self.env)
      outputs = [t.m_outputs[0] for t in self.compiled_tasks]
      self.link_task.set_inputs(outputs)
      self.link_task.set_outputs(self.path.find_build(ccroot.get_target_name(self)))
      # create header too
      headertask = self.create_task('name2h', self.env)
      headertask.set_inputs(input_nodes)
      headertask.set_outputs(self.path.find_build(self.target+'.h'))

def build_h(task):
  # build h file from input file namesa
  env = task.env()
  filename = task.m_outputs[0].abspath(env)
  fp  = open(filename, 'w')
  target = os.path.basename(filename).replace('.', '_')
  fp.write("""#ifndef %s_seen
#define %s_seen
#ifdef __cplusplus
extern "C" {
#endif\n"""%(target, target))
  for i in task.m_inputs:
    fd = os.path.basename(i.srcpath(env)).replace('.', '_')
    fp.write("""
            extern const u16 _binary_%s_bin_end[];
            extern const u16 _binary_%s_bin_start[];
            extern const u32 _binary_%s_bin_size[];
            """%(fd, fd, fd))
  fp.write("""
#ifdef __cplusplus
};
#endif
#endif
""")
  return 0

def detect(conf):
  v = conf.env
  dka_bin='%s/bin'%Params.g_options.devkitarm
  arm_eabi='arm-eabi-objcopy'
  objcopy = conf.find_program(arm_eabi, path_list=[dka_bin], var='OBJCOPY')
  if not objcopy:
    conf.fatal('objcopy was not found')
  v['OBJCOPY'] = objcopy

def setup(bld):
  objcopy_str = '${OBJCOPY} -O binary ${SRC} ${TGT}'
  Action.simple_action('objcopy', objcopy_str, color='BLUE', prio=130)
  objcopy_str = 'cd ${SRC[0].bld_dir(env)} && ${OBJCOPY} ${OBJCOPYFLAGS} ${SRC[0].m_name} ${TGT[0].m_name}'
  Action.simple_action('bin2o', objcopy_str, color='BLUE', prio=80)
  Action.Action('name2h', vars=[], func=build_h, prio=81)



