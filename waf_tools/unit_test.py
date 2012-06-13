""" Improved unit test module. """

import os
import TaskGen
import Task
import Logs

@TaskGen.feature('unit_test')
#@TaskGen.before('apply_core')
@TaskGen.after('apply_link', 'vars_target_cprogram')
def apply_unit_test(self):
    if not 'cprogram' in self.features:
        Logs.error('test cannot be executed %s' % self)
        return
    task = self.create_task('unit_test')
    task.set_inputs(self.link_task.outputs)
    ouput = os.path.basename(self.link_task.outputs[0].abspath(self.env))+'.passed'
    task.set_outputs(self.path.find_or_declare(ouput))
    task.run_from_dir = getattr(self, "run_from_dir", True)

def detect(conf):
    pass

def setup(bld):
    def run_unit_test(task):
        """ Execute unit tests, creating a test.passed cache file. Based on
         www.scons.org/wiki/UnitTest modified to run tests in the source dir """
        import subprocess
        import Options
        test_exe = task.inputs[0]
        app = test_exe.abspath(task.env)
        blddir = os.path.dirname(app)
        srcdir = os.path.dirname(test_exe.abspath())
        cwd = None
        if task.run_from_dir:
            cwd = task.run_from_dir
        target = os.path.join(blddir, task.outputs[0].name)
        process_pipe = subprocess.Popen(app, stdout=subprocess.PIPE, cwd=cwd)
        process_pipe.wait()
        returncode = process_pipe.returncode
        stdout = process_pipe.stdout.read()
        if Options.options.verbose or returncode:
            if task.run_from_dir:
                # this sorts out jump-to-errors in e.g. vim
                print "waf: Entering directory `%s'" % (bld.bdir)
            print stdout
        out = 'FAILED'
        if returncode == 0:
            out = stdout
        open(str(target),'w').write(out+"\n")
        return returncode
    Task.task_type_from_func('unit_test', vars=[], func=run_unit_test, color='RED', after="cxx_link")
