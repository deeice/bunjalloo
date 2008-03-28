import os, sys
import Configure

DEVKITPRO="DEVKITPRO"
DEVKITARM="DEVKITARM"
def setup_tool(conf, proc):
  if not os.environ.has_key(DEVKITPRO):
    conf.fatal('No DEVKITPRO variable set.')
  if not os.environ.has_key(DEVKITARM):
    conf.fatal('No DEVKITARM variable set.')

  v = conf.env
  ARCH = '-mthumb -mthumb-interwork'

  dka_bin='%s/bin'%os.environ[DEVKITARM]
  arm_eabi='arm-eabi-'

  # reset the tools
  for i in ( ('cpp','CPP'), ('gcc','CC'), ('g++','CXX'), ('ar','AR'), ('ranlib','RANLIB')):
    t = conf.find_program(arm_eabi+i[0], path_list=[dka_bin])
    if not t:
      conf.fatal(i[0]+' was not found')
    v[i[1]] = t

  v['ARFLAGS'] = 'rc'
  v['LINK_CC'] = v['CC']
  v['CCFLAGS_ARM%d'%proc] = (ARCH+' -g -ffast-math').split()
  v['CCFLAGS_ARM%d'%proc].extend({9:' -march=armv5te -mtune=arm946e-s -DARM9',
      7:' -mcpu=arm7tdmi -mtune=arm7tdmi -DARM7 '}[proc].split())
  v['CPPFLAGS_ARM%d'%proc] = ' -g -fno-rtti -fno-exceptions '.split()
  path = ['%s/libnds/include'%os.environ[DEVKITPRO]]
  #v['CPPPATH'] = path
  v['CPPPATH_ARM%d'%proc] = path
  v['LIBPATH_ARM%d'%proc] = ['%s/libnds/lib'%os.environ[DEVKITPRO]]
  v['LINKFLAGS_ARM%d'%proc] = ['-specs=ds_arm%d.specs'%proc,
      '-g','-mno-fpu','-Wl,-Map,map%d.map'%proc, '-Wl,-gc-sections']
  if proc == 9:
    v['LIB_ARM%d'%proc] = ['fat', 'nds%d'%proc]
  else:
    v['LIB_ARM%d'%proc] = ['nds%d'%proc]
  v['LINK_CXX'] = v['CXX']
  #v['CXXPATH'] = v['CPPPATH']
  v['CPPFLAGS_ARM%d'%proc].extend(v['CCFLAGS_ARM%d'%proc])
  v['CXXDEFINES'] = v['CCDEFINES']


