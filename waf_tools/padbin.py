#!/usr/bin/python
import os
from TaskGen import feature, before
import Utils
import Task
import misc

@feature('padbin')
@before('apply_core')
def apply_pb(self):
    self.meths.remove('apply_core')
    get_node = self.path.find_or_declare
    get_source = self.path.find_resource

    task = self.create_task('padbin')
    task.set_inputs(get_source(self.source))
    task.set_outputs(get_node(self.target))

def setup(bld):
    padbin_str = "cp ${SRC} ${TGT} && ${PADBIN} ${PADBINFLAGS} ${TGT}"
    Task.simple_task_type('padbin', padbin_str, color='BLUE', before='cxx cc', after='sox')

def detect(conf):
    import devkitarm
    dka_bin = devkitarm.get_devkitarm_bin()
    conf.find_program('padbin', path_list=[dka_bin], var='PADBIN', mandatory=1)
    if 'PADBINFLAGS' not in conf.env:
        conf.env['PADBINFLAGS'] = ['4']
