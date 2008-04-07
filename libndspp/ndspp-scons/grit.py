"""
  Copyright (C) 2007,2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
"""  SCons tool specification for grit """

import os.path
import color
from SCons.Builder import Builder
DEVKITARM = 'DEVKITARM'

def getGrit(env):
  gritInPath = env.WhereIs('grit')
  if gritInPath:
    return gritInPath
  else:
    import os
    if not os.environ.has_key(DEVKITARM):
      color.pprint('''*** Could not find "grit" in the PATH.''', color.red)
      color.pprint('Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM', color.red)
      env.Exit(1)
    else:
      gritInDKA = env.WhereIs('grit', os.path.join(os.environ[DEVKITARM], 'bin'))
      if gritInDKA:
        return gritInDKA

  color.pprint('''*** Could not find "grit" in the PATH.''', color.red)
  print 'Make sure you are using the latest DevkitArm release'
  env.Exit(1)

def exists(env):
  # perhaps it is in the path already
  grit = getGrit(env)
  return grit != None

def generate(env, **kw):
  #
  grit = getGrit(env)
  if grit:
    env['GRIT'] = grit
    if not env.has_key('GRITFLAGS'):
      env.Append(GRITFLAGS='-pw 16 -gB 8 -m! -ft b -fh! -q')

    ## Create bin files from png.
    # This adds an "emitter" that tells the Builder what things are made,
    # allowing 1 to many rules (png -> map, pal, img)
    # @param target the target artifacts
    # @param source the source input
    # @param env the environment
    # @return target, source list tuple
    def img_emitter(target, source, env):
      # add img.bin, pal.bin and map.bin to the output as side effects
      fileName = os.path.basename(str(source[0])).split(os.path.extsep)[0]
      t = []
      t.append('%s.img.bin'%(fileName))
      t.append('%s.pal.bin'%(fileName))
      # t.append('%s.map.bin'%(fileName))
      return (t, source)
    # chdir=1 changes to the target dir before executing.
    gritBld = Builder(action='$GRIT ${SOURCE.abspath} -o ${SOURCE.file} $GRITFLAGS >/dev/null',
        chdir=1,
        suffix='.bin',
        src_suffix='.png',
        emitter=img_emitter)
    env.Append( BUILDERS = { 'Gfx2Bin': gritBld } )
