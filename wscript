#!/usr/bin/python
import sys
sys.path.append('waf_tools')
import scwaf
import Scripting
import UnitTest
import Options
blddir = '_build_'

def set_options(opt):
    opt.tool_options('compiler_cxx')
    opt.tool_options('compiler_cc')
    opt.tool_options('eclipse_cdt')
    opt.tool_options('google_profiler')
    opt.tool_options('gprof')
    opt.tool_options('gcov')
    opt.sub_options('waf_tools')

def configure(conf):
    config_h = 'bunjalloo/bwt/include/config_defs.h'
    DATADIR='data/bunjalloo'
    conf.check_tool('grit')
    conf.check_tool('misc')
    conf.check_tool('sox')
    conf.check_tool('padbin')
    conf.addenv('nds', is_copy=True)
    conf.check_tool('eclipse_cdt')
    conf.sub_config('waf_tools')
    conf.check_tool('unit_test')
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')
    conf.check_tool('objcopy')
    conf.check_tool('google_profiler')
    conf.check_tool('gprof')
    if Options.options.with_gcov:
        conf.check_tool('gcov')
    conf.env['CXXFLAGS'] += '-g -O0'.split()
    conf.env['OBJCOPYFLAGS'] = ' -I binary -O elf32-i386 -B i386 '.split()
    conf.define('sprintf_platform', 'sprintf', quote=False)
    conf.define('sscanf_platform', 'sscanf', quote=False)
    conf.define('DATADIR', DATADIR)
    conf.write_config_header(config_h)
    common_libs = 'gif jpeg z png unzip matrixsslstatic'
    for lib in ('SDL GL '+common_libs).split():
        conf.check(lib=lib, mandatory=True)

    result = conf.check_cfg(
        path='gtest-config',
        package='',
        msg='Checking for gtest',
        args='--cppflags --ldflags --libs',
        uselib_store='GTEST')
    if result:
        conf.env['LIB_GTEST'].append('gtest_main')

    conf.setenv('nds')
    conf.check_tool('devkitarm')
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')
    conf.check_tool('objcopy')
    conf.check_tool('ndstool')
    conf.check_libnds()
    for lib in ('dswifi9 '+common_libs).split():
        conf.check(lib=lib, uselib='NDS', mandatory=True)
    conf.define('sprintf_platform', 'siprintf', quote=False)
    conf.define('sscanf_platform', 'siscanf', quote=False)
    conf.define('DATADIR', DATADIR)
    conf.write_config_header(config_h)
    if Options.options.generate_eclipse:
        Scripting.commands.append('build')

def build(bld):
    if 'check' in Scripting.commands:
        bld.add_subdirs('waf_tools')
    for env in bld.all_envs:
        bld.set_variant(env)
        bld.add_subdirs('libndspp')
        bld.add_subdirs('bunjalloo')
        bld.add_subdirs('ChaosDS')
    bld.check_generate_eclipse()

def check(ctx):
    pass
