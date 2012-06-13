import sys
import Task
import Utils
import Options
from Constants import RUN_ME
from TaskGen import extension

def set_options(opt):
    opt.add_option('--all-tests',
        action='store_true',
        default=False,
        help='Exec all unit tests')
    opt.add_option('--test',
        action='store',
        help='Exec only this test')
    opt.add_option('--filter',
        action='store',
        help='Add this filter to the command line')

def exec_test(self):
    filename = self.inputs[0].abspath(self.env)
    f = getattr(Options.options, 'filter', '')
    cmd = [sys.executable, filename]
    if f:
        cmd += [f]
    Utils.cmd_output(cmd)

@extension('.py')
def run_test(self, node):
    task = self.create_task('waftest')
    task.set_inputs(node)
cls = Task.task_type_from_func('waftest', func=exec_test, color='RED', ext_in='.py')
old = cls.runnable_status
def test_status(self):
    for flag in ('all_tests', 'test'):
        if getattr(Options.options, flag, False):
            return RUN_ME
    return old(self)
cls.runnable_status = test_status
cls.quiet = 1

def detect(conf):
    return True
