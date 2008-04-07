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
"""  SCons tool specification for unit tests """

import os, os.path
from SCons.Builder import Builder

def exists(env):
  return True

def generate(env, **kw):
  def run_unit_test(target, source, env):
    """ Execute unit tests, creating a test.passed cache file. Based on
     www.scons.org/wiki/UnitTest modified to run tests the source dir. """
    dirname = str(source[0].srcnode().dir)
    app = str(source[0].abspath)
    cwd = os.getcwd()
    print "scons: Entering directory `%s'"%os.path.join(cwd,dirname)
    os.chdir(dirname)
    result = os.spawnl(os.P_WAIT, app, app)
    os.chdir(cwd)
    if result == 0:
      open(str(target[0]),'w').write("PASSED\n")
      return 0
    else:
      return 1
  # Create a builder for tests
  bld = Builder(action = run_unit_test)
  env.Append(BUILDERS = {'Test' :  bld})

