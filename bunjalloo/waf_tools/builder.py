#!/usr/bin/env python
# encoding: utf-8
import os

def build(bld, buildlib=True, buildsdl=True):
  APPNAME = os.path.basename(bld.m_curdirnode.srcpath(bld.env()))
  if buildlib:
    arm9font = bld.create_obj('bin2o')
    arm9font.source = """
                        common/fonts/vera.img
                        common/fonts/vera.map
                        common/fonts/vera.pal
                       """
    arm9font.target = 'vera'
    if buildsdl:
      sdlfont = arm9font.clone('sdl')

  arm9 = bld.create_obj('cpp', 'program')
  arm9.find_sources_in_dirs('arm9')
  arm9.includes += ' .'
  arm9.uselib_local = 'bwt'
  if buildlib:
    arm9.uselib_local += ' vera'

  arm9.uselib = 'ARM9'
  arm9.target = APPNAME+'-arm9'

  if buildsdl:
    sdl = arm9.clone('sdl')
    sdl.uselib = ''
    sdl.target = APPNAME
    check_target(sdl)

  arm9bin = bld.create_obj('objcopy')
  arm9bin.source = arm9.target
  arm9bin.target = arm9.target+'.bin'

  nds = bld.create_obj('ndstool')
  nds.source = arm9bin.target
  nds.target = APPNAME+'.nds'
  check_target(nds)

def check_target(obj):
  import Params, Utils
  if Params.g_options.delete_scons:
    for target in Utils.to_list(obj.target):
      t = os.path.join(obj.path.abspath(), target)
      if os.path.exists(t):
        Params.pprint('YELLOW', 'Deleting %s/%s'%(obj.path.srcpath(obj.env), target))
        os.unlink(t)

def build_test(bld):
  tst = bld.create_obj('cpp', 'program')
  tst.env = bld.env('sdl').copy()
  tst.target = 'tester'
  check_target(tst)
  tst.find_sources_in_dirs('.')
  tst.unit_test = 1
  tst.includes += ' .'
  tst.uselib_local = 'bwt cppunitmain'
  tst.uselib = 'TEST HOST'

  # Cached unit tests.
  ut = bld.create_obj('unit_test')
  ut.env = bld.env('sdl').copy()
  ut.source = tst.target

