import unittest
import waftester
import scwaf
import Options
from Build import BuildContext
import Build

class SCwafTest(unittest.TestCase):
    def testOptDecorator(self):
        @scwaf.option
        def foobar(opt):
            return 1
        self.assertTrue(Options.Handler.foobar)
        optionctx = Options.Handler()
        self.assertTrue(optionctx.foobar())

    def testBuildDecorator(self):
        @scwaf.build
        def foobar(bld):
            return 1
        self.assertTrue(BuildContext.foobar)
        bldctx = BuildContext()
        self.assertTrue(bldctx.foobar())

    def testCCStaticLib(self):
        features = scwaf.get_features('somefile.c other.c')
        self.assertEqual('cc', features)
        features = scwaf.get_features('somefile.cpp other.c')
        self.assertEqual('cxx', features)

def CompilerOptions(opt):
    opt.tool_options('compiler_cxx')

def CompilerConfigure(conf):
    conf.check_tool('compiler_cxx')

def StaticLibBuild(bld):
    obj = bld.static_library('test', 'source')
    obj.include_directories('include')

foo_h_data = """
int in_foo_h();
"""
foo_c_data = """
#include "foo.h"
int foo()
{
    in_foo_h();
    return 1;
}
"""

main_cpp_data = """
int main()
{
    return 0;
}
"""

def StaticLibBuild2(bld):
    obj = bld.static_library('test', 'source')
    obj.include_directories('include', export=False)

bar_c_data = """
int bar()
{
    return 1;
}
"""

baz_c_data = """
int baz()
{
    return 1;
}
"""
def StaticLibBuild3(bld):
    obj = bld.static_library('test', 'source')
    obj.source_directories('source2')

def BuildWithArgs(bld):
    obj = bld.static_library('test', 'source',
            FOOFLAGS = 'foobar',
            install_path = '/opt/test')

def BuildProgWithArgs(bld):
    bld.program('test', 'source')

class SCwafTestFiles(waftester.WafTestCase):

    def testStaticLib(self):
        self.suite.write_wscript(options=CompilerOptions, configure=CompilerConfigure, build=StaticLibBuild)
        self.suite.subdir('source')
        self.suite.subdir('include')
        self.suite.write_data(foo_c_data, 'source/foo.cpp')
        self.suite.write_data(foo_h_data, 'include/foo.h')
        self.suite.run_waf()

        staticlib_PATTERN = Build.bld.env['staticlib_PATTERN']
        self.assertTrue(self.suite.exists(staticlib_PATTERN % 'test'))
        exports = Build.bld.all_task_gen[0].export_incdirs
        self.assertEquals(sorted(['include', 'source']), sorted(exports))

    def testExportInc(self):
        self.suite.write_wscript(options=CompilerOptions, configure=CompilerConfigure, build=StaticLibBuild2)
        self.suite.subdir('source')
        self.suite.subdir('include')
        self.suite.write_data(foo_c_data, 'source/foo.cpp')
        self.suite.write_data(foo_h_data, 'include/foo.h')
        self.suite.run_waf()
        staticlib_PATTERN = Build.bld.env['staticlib_PATTERN']
        self.assertTrue(self.suite.exists(staticlib_PATTERN % 'test'))
        exports = Build.bld.all_task_gen[0].export_incdirs
        self.assertEquals(['source'], exports)

    def testAddSourceDir(self):
        self.suite.write_wscript(options=CompilerOptions, configure=CompilerConfigure, build=StaticLibBuild3)
        self.suite.subdir('source')
        self.suite.subdir('source2')
        self.suite.write_data(bar_c_data, 'source/bar.cpp')
        self.suite.write_data(baz_c_data, 'source2/baz.cpp')
        self.suite.run_waf()
        staticlib_PATTERN = Build.bld.env['staticlib_PATTERN']
        self.assertTrue(self.suite.exists(staticlib_PATTERN % 'test'))
        exports = Build.bld.all_task_gen[0].export_incdirs
        self.assertEquals(sorted(['source', 'source2']), sorted(exports))

    def testStaticLibArgs(self):
        self.suite.write_wscript(options=CompilerOptions, configure=CompilerConfigure, build=BuildWithArgs)
        self.suite.subdir('source')
        self.suite.write_data(bar_c_data, 'source/bar.cpp')
        self.suite.run_waf()
        taskgen = Build.bld.all_task_gen[0]
        self.assertEqual('foobar', taskgen.env['FOOFLAGS'])
        self.assertEqual('/opt/test', taskgen.install_path)

    def testProgram(self):
        self.suite.write_wscript(options=CompilerOptions, configure=CompilerConfigure, build=BuildProgWithArgs)
        self.suite.subdir('source')
        self.suite.write_data(main_cpp_data, 'source/main.cpp')
        self.suite.run_waf()
        program_PATTERN = Build.bld.env['program_PATTERN']
        self.assertTrue(self.suite.exists(program_PATTERN % 'test'))

class SCwafTestFunctions(unittest.TestCase):

    def testParseTaskGenArgs(self):
        args = {'foo':'foo_value',
                'BAR12_V': 'bar_value'}
        env, attr = scwaf.parse_task_gen_args(args)
        self.assertEquals({'BAR12_V': 'bar_value'} , env)
        self.assertEquals({'foo': 'foo_value'} , attr)

    def testApplyEnv(self):
        env = {'BAR_123': 'bar_value', 'FOO_456':'foo_value'}
        class FakeTG:
            env = {}
        fake = FakeTG()
        scwaf.apply_env_values(fake, env)
        self.assertEquals('bar_value', fake.env['BAR_123'])
        self.assertEquals('foo_value', fake.env['FOO_456'])

def VariantConf(conf):
    conf.addenv('debug')
    CompilerConfigure(conf)
    conf.setenv('debug')
    CompilerConfigure(conf)

    conf.addenv('another', is_copy=True)

def DoNothing(bld):
    for env in sorted(bld.all_envs):
        bld.set_variant(env)
        lib = bld.static_library('test', '.')

class SCwafTestConfig(waftester.WafTestCase):

    def testVariants(self):
        self.suite.write_wscript(options=CompilerOptions, configure=VariantConf, build=DoNothing)
        self.suite.write_data(bar_c_data, 'bar.cpp')
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('bar_1.o', 'another'), "another bar.o")
        self.assertTrue(self.suite.exists('bar_2.o', 'debug'), "debug bar.o")
        self.assertTrue(self.suite.exists('bar_3.o'), "default bar.o")
        tasks = Build.bld.all_task_gen
        self.assertEqual('another', tasks[0].env.variant())
        self.assertEqual('debug', tasks[1].env.variant())
        self.assertEqual('default', tasks[2].env.variant())


if __name__ == '__main__':
    unittest.main()
