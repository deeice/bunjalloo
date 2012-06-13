#!/usr/bin/python
from TaskGen import feature, before
import Task
import os
import Utils
from scwaf import build, apply_env_values, parse_task_gen_args

@feature('objcopy')
@before('apply_core')
def apply_objcopy(self):
    self.meths.remove('apply_core')
    task = self.create_task('objcopy')
    node = self.path.find_or_declare(self.source)
    task.set_inputs(node)
    task.set_outputs(node.change_ext('.o'))
    self.compiled_tasks = [task]

@build
def objcopy(self, source, **kwargs):
    env, attr = parse_task_gen_args(kwargs)
    tg = self.new_task_gen(features='objcopy', source=source, **attr)
    apply_env_values(tg, env)
    return tg

@feature('name2h')
@before('apply_core')
def apply_name2h(self):
    self.meths.remove('apply_core')
    find_resource = self.path.find_resource
    find_target = self.path.find_or_declare
    task = self.create_task('name2h')
    task.set_inputs([find_target(filename) for filename in self.to_list(self.source)])
    task.set_outputs(find_target(self.target))

header_template = '''\
#ifndef %s_seen
#define %s_seen
#ifdef __cplusplus
extern "C" {
#endif
'''

binary_template = """
    extern const u16 _binary_%s_end[];
    extern const u16 _binary_%s_start[];
    extern const u32 _binary_%s_size[];
"""

footer_template = """
#ifdef __cplusplus
};
#endif
#endif
"""

def build_h(task):
    env = task.env
    filename = task.outputs[0].abspath(env)
    hfile = open(filename, 'w')
    guard = os.path.basename(filename).replace('.', '_')
    hfile.write(header_template % ((guard,)*2))
    symbols = sorted(os.path.basename(i.srcpath(env)).replace('.', '_')
            for i in task.inputs)
    for symbol in symbols:
        hfile.write(binary_template % ((symbol,)*3))
    hfile.write(footer_template)
    return 0

def setup(bld):
    objcopy_str = 'cd ${SRC[0].bld_dir(env)} && ${OBJCOPY} ${OBJCOPYFLAGS} ${SRC[0].name} ${TGT[0].name}'
    Task.simple_task_type('objcopy', objcopy_str, color='YELLOW', before='cxx cc', after='padbin copy')
    Task.task_type_from_func('name2h', vars=[], func=build_h, before='cc cxx', after='padbin copy')

def detect(conf):
    conf.find_program('objcopy', var='OBJCOPY', mandatory=True)
