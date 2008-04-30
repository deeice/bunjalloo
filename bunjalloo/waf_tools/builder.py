""" Build test programs """
import os

def build(bld, buildlib=True, buildsdl=True):
  """
  Build a test program, optionally building the library and optionally
  building for SDL
  """
  app_name = os.path.basename(bld.m_curdirnode.srcpath(bld.env()))
  if buildlib:
    arm9font = bld.create_obj('bin2o')
    arm9font.source = """
                        common/fonts/vera.img
                        common/fonts/vera.map
                        common/fonts/vera.pal
                       """
    arm9font.target = 'vera'
    arm9font.inst_var = 0
    if buildsdl:
      arm9font.clone('sdl')

  arm9 = bld.create_obj('cpp', 'program')
  arm9.inst_var = 0
  arm9.find_sources_in_dirs('arm9')
  arm9.includes += ' .'
  libs = 'bwt'
  if buildlib:
    libs += ' vera'
  arm9.uselib_local = libs.split()

  arm9.uselib = 'ARM9'
  arm9.target = app_name+'-arm9'

  if buildsdl:
    sdl = arm9.clone('sdl')
    sdl.uselib = 'HOST'
    sdl.target = app_name
    check_target(sdl)

  arm9bin = bld.create_obj('objcopy')
  arm9bin.inst_var = 0
  arm9bin.source = arm9.target
  arm9bin.target = arm9.target+'.bin'

  nds = bld.create_obj('ndstool')
  nds.source = arm9bin.target
  nds.target = app_name+'.nds'
  check_target(nds)

def check_target(obj):
  """ Check for build targets in the source tree, and delete them """
  import Params, Utils
  if Params.g_options.delete_scons:
    for target in Utils.to_list(obj.target):
      abs_target = os.path.join(obj.path.abspath(), target)
      if os.path.exists(abs_target):
        Params.pprint('YELLOW',
            'Deleting %s/%s' % (obj.path.srcpath(obj.env), target))
        os.unlink(abs_target)

def build_test(bld):
  """ Build a unit test program """
  tst = bld.create_obj('cpp', 'program')
  tst.inst_var = 0
  tst.env = bld.env('sdl').copy()
  tst.target = 'tester'
  check_target(tst)
  tst.find_sources_in_dirs('.')
  tst.unit_test = 1
  tst.includes += ' .'
  tst.uselib_local = 'bunjalloo bwt cppunitmain'
  tst.uselib = 'TEST HOST'

  # Cached unit tests.
  unit_test = bld.create_obj('unit_test')
  unit_test.env = bld.env('sdl').copy()
  unit_test.source = tst.target

