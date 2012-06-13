import tempfile
import os
import unittest
import sys
import shutil
import StringIO
sys.path.append(os.environ['WAFDIR'])
sys.path.append(os.path.join(os.environ['WAFDIR'], 'Tools'))

class WafTestException(Exception):
    pass

def get_function(name, function):
    data = ''
    if function:
        func_path, func_file = os.path.split(function.func_code.co_filename)
        func_file = func_file.split('.')[0]
        if not func_path:
            func_path = os.path.abspath(os.path.dirname(__file__))
        data += 'sys.path.append("%s")\n'%(func_path)
        data += 'def %s(ctx):\n'%name
        data += '    from %s import %s\n'%(func_file, function.__name__)
        data += '    %s(ctx)\n'%(function.__name__)
    return data

class WafTestCase(unittest.TestCase):
    def setUp(self):
        self.suite = TestWaf()

    def tearDown(self):
        self.suite.done()

    def CompareContents(self, expected, filename):
        if type(filename) == str:
            data = open(filename).readlines()
        else:
            data = filename.readlines()
        expected_data = expected.split('\n')
        for x in zip(expected_data, data):
            x0 = x[0].strip()
            x1 = x[1].strip()
            self.assertEquals(x0, x1)


def DoNothing(bld):
    pass

def CompilerOptions(opt):
    opt.tool_options('compiler_cxx')
    opt.tool_options('compiler_cc')

class TestWaf:
    def __init__(self):
        self.basedir = tempfile.mkdtemp()

    def write_data(self, data, filename):
        f = os.path.join(self.basedir, filename)
        fp = open(f, 'w')
        fp.write(data)
        fp.close()

    def write_wscript(self, filename='wscript', options=None, configure=None, build=None):
        data = "import sys\n"
        data += get_function('set_options', options)
        data += get_function('configure', configure)
        data += get_function('build', build)
        self.write_data(data, filename)

    def subdir(self, name):
        os.mkdir(os.path.join(self.basedir, name))

    def exists(self, name, variant='default'):
        filename = os.path.join(self.basedir, 'build', variant, name)
        return os.path.exists(filename)

    def get_file(self, name, variant='default'):
        filename = os.path.join(self.basedir, 'build', variant, name)
        return open(filename)

    def run_waf(self, args=None):
        start = os.getcwd()
        old_argv = sys.argv
        old_stderr = sys.stderr
        mystderr = StringIO.StringIO()
        try:
            os.chdir(self.basedir)
            cwd = os.getcwd()
            sys.stderr = mystderr
            import Scripting
            import Utils
            import Constants
            Utils.get_term_cols = lambda: 80
            wafdir = os.environ['WAFDIR']
            tools = os.path.join(wafdir, 'Tools')
            sys.argv = ['', 'configure', 'build', '-v']
            if args:
                sys.argv += args
            sys.path = [wafdir, tools] + sys.path
            Scripting.prepare(tools, cwd, Constants.WAFVERSION, wafdir)
        except SystemExit:
            raise WafTestException(mystderr.getvalue())
        finally:
            os.chdir(start)
            sys.argv = old_argv
            sys.stderr = old_stderr

    def done(self):
        shutil.rmtree(self.basedir)
        #print self.basedir
