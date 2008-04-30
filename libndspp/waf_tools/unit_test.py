""" Improved unit test module. """

import Object

class unit_test_taskgen(Object.task_gen):
  def __init__(self, *k, **kw):
    Object.task_gen.__init__(self, *k, **kw)
    self.run_from_srcdir = True

  def apply(self):
    find_build = self.path.find_build
    input_nodes = []
    for filename in self.to_list(self.source):
      node = find_build(filename)
      input_nodes.append(node)
    task = self.create_task('unit_test', self.env)
    task.run_from_srcdir = self.run_from_srcdir
    task.set_inputs(input_nodes)
    task.set_outputs(find_build('test.passed'))

def detect(conf):
  pass

def setup(bld):
  def run_unit_test(task):
    """ Execute unit tests, creating a test.passed cache file. Based on
     www.scons.org/wiki/UnitTest modified to run tests the source dir. """
    import os, subprocess
    import Params
    test_exe = task.m_inputs[0]
    app = test_exe.abspath(task.env())
    blddir = os.path.dirname(app)
    srcdir = os.path.dirname(test_exe.abspath())
    cwd = os.getcwd()
    if task.run_from_srcdir:
      os.chdir(srcdir)
    target = os.path.join(blddir, task.m_outputs[0].m_name)
    process_pipe = subprocess.Popen(app, stdout=subprocess.PIPE)
    process_pipe.wait()
    returncode = process_pipe.returncode
    if Params.g_verbose or returncode:
      if task.run_from_srcdir:
        # this sorts out jump-to-errors in e.g. vim
        print "waf: Entering directory `%s'" % (srcdir)
      print process_pipe.stdout.read()
    if task.run_from_srcdir:
      os.chdir(cwd)
    out = 'FAILED'
    if returncode == 0:
      out = 'PASSED'
    open(str(target),'w').write(out+"\n")
    return returncode
  import Action
  Action.Action('unit_test', vars=[], func=run_unit_test, color='PINK', prio=199)
