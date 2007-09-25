""" SCons tool specification for DevkitArm """
from SCons.Builder import Builder
from SCons.Defaults import Copy

import os, os.path, sys

DEVKITARM = 'DEVKITARM'
DEVKITPRO = 'DEVKITPRO'
devkitPrefix = 'arm-eabi-'

# define global variables
thumbFlags = ' -mthumb -mthumb-interwork '
processorCCFlags = {
  'arm9': ' -march=armv5te -mtune=arm946e-s',
  'arm7': ' -mcpu=arm7tdmi -mtune=arm7tdmi'
}

processorLDFlags = ' -specs=ds_arm%c.specs -g -mno-fpu -Wl,-Map,${TARGET.base}.map'

cxxFlags = ' -fno-rtti -fno-exceptions '
extraflags = ' -Wno-strict-aliasing \
               -fomit-frame-pointer -ffast-math '

def getLibnds():
  if not os.environ.has_key(DEVKITPRO):
    print 'No DEVKITPRO variable set. libnds will probably not be found...'
    return ''
  else:
    return os.path.sep.join( [ os.environ[DEVKITPRO], 'libnds'] )

def devkitArmSet():
  if not os.environ.has_key(DEVKITARM):
    print 'Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM'
    sys.exit(1)
    return 0
  return 1

def addBinBuilder(env):
  # add the final ndstool step
  def generate_nds(source, target, env, for_signature):
    banner = ''
    if env.has_key('BANNER'):
      banner = env['BANNER']
    if env.has_key('ICON'):
      return """ndstool -c %s -7 %s -9 %s -b %s "%s" \
	  > /dev/null """%(target[0], source[0], source[1], env['ICON'], banner)
    else:
      return """ndstool -c %s -7 %s -9 %s > /dev/null """%(target[0], source[0], source[1], source[2])
  bld = Builder(generator=generate_nds, suffix='.nds')
  env.Append( BUILDERS = { 'NdsTool': bld } )

  def generate_arm(source, target, env, for_signature):
    return '$OBJCOPY -O binary %s %s'%(source[0], target[0])
  armBuild = Builder(generator=generate_arm, suffix='.arm', src_suffix='.elf')
  env.Append( BUILDERS = { 'Elf2Arm': armBuild } )

  env.Append(DLDITOOL='dlditool')
  env.Append(DLDIFILE='none.dldi')
  dldiPatch = Builder( action=[Copy('$TARGET', '$SOURCE'), '-$DLDITOOL $DLDIFILE $TARGET'],
      suffix='.nds', src_suffix='.nds')
  env.Append( BUILDERS = { 'DldiPatch': dldiPatch } )

def generate(env, **kw):
  global devkitPrefix
  # fetch the processor type, or arm9 by default
  armType = kw.get('type', 'arm9').lower()
  if (devkitArmSet()):
    devkitArmBin = ( os.path.sep.join( [ os.environ[DEVKITARM], 'bin'] ) )
    env.PrependENVPath('PATH', devkitArmBin)
  ccflags = ' '.join( [ extraflags, thumbFlags, processorCCFlags[armType] ] )
  env.Append(CCFLAGS=ccflags)
  env.Append(CXXFLAGS=cxxFlags)
  env.Append(CPPDEFINES='%s'%armType.upper())
  env.Append(CPPPATH=os.path.sep.join( [getLibnds(), 'include'] ))
  env['CC'] = devkitPrefix+'gcc'
  env['CXX'] = devkitPrefix+'g++'
  env['AS'] = devkitPrefix+'as'
  env['AR'] = devkitPrefix+'ar'
  env['OBJCOPY'] = devkitPrefix+'objcopy'
  # add 9 or 7
  c = armType[ len(armType) - 1 ]
  if (c == '9'):
    env.Append(LIBS=['fat'])
  env.Append(LIBS=['nds%c'%c])
  env.Append(LIBS=['dswifi%c'%c])

  env.Append(LIBPATH=[os.path.sep.join( [getLibnds(), 'lib'] )])
  env.Append(LINKFLAGS=processorLDFlags%c)
  env.Append(LINKFLAGS=thumbFlags)
  addBinBuilder(env)



def exists(env):
  return devkitArmSet()

