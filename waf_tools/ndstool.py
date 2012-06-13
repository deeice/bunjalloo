#!/usr/bin/python
"""
Waf tool for generating a .nds file from 1 or 2 arm cores, optionally using a
banner.
"""
import os
import sys
from scwaf import build, apply_env_values, parse_task_gen_args
from TaskGen import feature, after, before
import Task
import Utils
import devkitarm

@build
def create_nds(self, target, source, **kwargs):
    env, attr = parse_task_gen_args(kwargs)
    tg = self.new_task_gen(
            features='nds',
            target=target,
            source=source,
            **attr)
    apply_env_values(tg, env)

@feature('nds')
@before('apply_core')
def create_nds_feature(self):
    """
    build/target.arm: build/target
        objcopy flags $< $@
    build/target.nds: build/target.arm [source/icon banner]
        ndstool -c $@ -9 $<  [-b source/icon banner]
    """
    self.meths.remove('apply_core')

    get_node = self.path.find_or_declare
    get_source = self.path.find_resource
    elf2bin_in = [get_node(filename) for filename in
            Utils.to_list(self.source)]
    binfile = elf2bin_in[0].change_ext('.arm')
    ndstask_inputs = [binfile]

    ndstask = 'ndstool'
    if getattr(self, 'banner', None):
        ndstask += '_banner'
        self.env['NDSTOOL_BANNER'] = self.banner
        if getattr(self, 'icon', None):
            ndstask_inputs += [get_source(self.icon)]
        else:
            raise Utils.WscriptError("ndstool requires an icon with the banner")

    task = self.create_task('elf2bin')
    task.set_inputs(elf2bin_in)
    task.set_outputs(binfile)
    self.tasks.append(task)

    os.environ['DEVKITPRO'] = self.env['DEVKITPRO']
    task = self.create_task(ndstask)
    task.set_inputs(ndstask_inputs)
    task.set_outputs(get_node(self.target))
    self.tasks.append(task)

def setup(bld):
    outfile = ''
    if sys.platform.startswith('linux'):
        outfile = ' > /dev/null'
    ndstool_str = '${NDSTOOL} -c ${TGT} -9 ${SRC[0].bldpath(env)} %s '+outfile
    Task.simple_task_type('ndstool', ndstool_str%'', before='unit_test', after='elf2bin')
    Task.simple_task_type('ndstool_banner',
            ndstool_str%" -b ${SRC[1].srcpath(env)} '${NDSTOOL_BANNER}'",
            before='unit_test',
            after='elf2bin')
    Task.simple_task_type('elf2bin',
            '${OBJCOPY} -O binary ${SRC} ${TGT}',
            after='cc_link cxx_link')

def detect(conf):
    dka_bin = devkitarm.get_devkitarm_bin()
    ndstool = conf.find_program('ndstool', path_list=[dka_bin], var='NDSTOOL', mandatory=True)

def generate_banner(app, comment='', author=None):
    if author == None:
        author = '?'
        try:
            import pwd
            author = pwd.getpwnam(os.getlogin()).pw_gecos.split(',')[0]
        except:
            pass
    banner = '%s;%s;By %s'%(app.title(), comment, author)
    if sys.platform.startswith('win'):
        banner = banner.replace(' ', '_')
    return banner
