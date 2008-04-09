import os, sys
import Configure, Params

DEVKITPRO="DEVKITPRO"
DEVKITARM="DEVKITARM"

def set_options(opt):
  dkp = ''
  dka = ''
  if DEVKITPRO in os.environ:
    dkp = os.environ[DEVKITPRO]

  if DEVKITARM in os.environ:
    dka = os.environ[DEVKITARM]
  elif dkp:
    dka = os.path.join(dkp, 'devkitARM')

  dkpo = opt.add_option_group("DevkitPro options")
  dkpo.add_option('--with-devkitpro', type='string',
          help='Path to devkitpro. Will use env DEVKITPRO by default.', default=dkp, dest='devkitpro')
  dkpo.add_option('--with-devkitarm', type='string',
          help='Path to devkitarm. Will use env DEVKITARM by default.', default=dka, dest='devkitarm')

def setup_tool(conf, proc):
  if not Params.g_options.devkitpro:
    conf.fatal('''No DEVKITPRO variable set. Set the path to devkit pro or use the --with-devkitpro option''')
  if not Params.g_options.devkitarm:
    Params.g_options.devkitarm = os.path.join(Params.g_options.devkitpro, 'devkitARM')
    Params.niceprint(
    '''No DEVKITARM variable set. Using %s'''%Params.g_options.devkitarm,
    'WARNING', 'Configuration')

  v = conf.env
  ARCH = '-mthumb -mthumb-interwork'

  dka_bin='%s/bin'%Params.g_options.devkitarm
  arm_eabi='arm-eabi-'

  # reset the tools
  for i in ( ('cpp','CPP'), ('gcc','CC'), ('g++','CXX'), ('ar','AR'), ('ranlib','RANLIB')):
    t = conf.find_program(arm_eabi+i[0], path_list=[dka_bin])
    if not t:
      conf.fatal(i[0]+' was not found')
    v[i[1]] = t

  v['LINK_CC'] = v['CC']
  v['LINK_CXX'] = v['CXX']

  v['CCFLAGS_ARM%d'%proc] = (ARCH+' -g -ffast-math').split()
  v['CCFLAGS_ARM%d'%proc].extend({9:' -march=armv5te -mtune=arm946e-s -DARM9',
      7:' -mcpu=arm7tdmi -mtune=arm7tdmi -DARM7 '}[proc].split())
  v['CPPFLAGS_ARM%d'%proc] = ' -g -fno-rtti -fno-exceptions '.split()
  path = ['%s/libnds/include'%Params.g_options.devkitpro]
  v['CPPPATH_ARM%d'%proc] = path
  v['LINKFLAGS_ARM%d'%proc] = ['-specs=ds_arm%d.specs'%proc,
      '-g','-mno-fpu','-Wl,-Map,map%d.map'%proc, '-Wl,-gc-sections'] + ARCH.split()
  # check for libnds
  e = conf.create_library_configurator()
  e.nosystem = 1
  e.mandatory = 1
  e.uselib = 'ARM%d'%proc
  e.path = ['%s/libnds/lib'%Params.g_options.devkitpro]
  e.name = 'nds%d'%proc
  e.run()
  if proc == 9:
    e.name = 'fat'
    e.run()
  # swap the last 2 over so linking works.
  v['LIB_ARM%d'%proc].reverse()
  v['CPPFLAGS_ARM%d'%proc].extend(v['CCFLAGS_ARM%d'%proc])
  v['CXXDEFINES'] = v['CCDEFINES']

