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
import os.path, sys

black = '\x1b[0m'
green = '\x1b[92m'
blue = '\x1b[94m'
red = '\x1b[91m'

def reset_colors():
  global black, green, blue, red
  black = ''
  green = ''
  blue = ''
  red = ''

def pprint(msg, col):
  print col + msg + black

def generate(env, **kw):
  """ Generate a coloured output environment """
  def print_cmd_line(s, targets, sources, env):
    # print s
    sourceTxt = ','.join( [ os.path.basename(str(f)) for f in sources] )
    if len(sourceTxt) > 80:
      sourceTxt = sourceTxt[0:75]+'%s...'%black
    prog = os.path.basename(s.replace('(', ' ').split()[0])
    sourceTxt = sourceTxt.replace(',', '%s,%s'%(black, blue))
    sys.stdout.write('%s:\t%s%s%s -> %s%s%s...\n'%
        (prog,blue, sourceTxt, black,
         green,('%s,%s'%(black, green)).join( [ os.path.basename(str(f)) for f in targets] ), black ))

  if not os.environ.has_key('NO_SCONS_COLOR'):
    env.Append(PRINT_CMD_LINE_FUNC=print_cmd_line)

  if not sys.stdout.isatty():
    reset_colors()

def exists(env):
  return 1
