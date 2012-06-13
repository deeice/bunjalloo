import os
import unittest
import waftester
import devkitarm
import Build
import Utils

def DKAConf(conf):
    conf.check_tool('devkitarm')

def DKABuild(bld):
    pass

def DKAWithCompiler(conf):
    conf.check_tool('devkitarm')
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')

def DKAWithLibnds(conf):
    DKAWithCompiler(conf)
    conf.check_libnds()

class TestDevkitArm(waftester.WafTestCase):

    def testDevkitArmUnset(self):
        dka = os.environ.pop('DEVKITARM')
        dkp = os.environ.pop('DEVKITPRO')
        try:
            self.suite.write_wscript(configure=DKAConf, build=DKABuild)
            # this shouldn't raise an error
            self.suite.run_waf()
            # and CC, etc, should not be set
            self.assertFalse('CC' in Build.bld.env)
            self.assertFalse('CXX' in Build.bld.env)
            self.assertFalse('CPP' in Build.bld.env)
            self.assertFalse('AR' in Build.bld.env)
            self.assertFalse('RANLIB' in Build.bld.env)
            self.assertFalse('OBJCOPY' in Build.bld.env)
        finally:
            os.environ['DEVKITARM'] = dka
            os.environ['DEVKITPRO'] = dkp

    def testDevkitArmCompiler(self):
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=DKAWithCompiler, build=DKABuild)
        self.suite.run_waf()
        # ensure the variables are what we expect...
        def prog(name):
            return [os.path.join(os.environ['DEVKITARM'],
                'bin', 'arm-eabi-'+name)]
        def env(val):
            return Utils.to_list(Build.bld.env[val])
        self.assertEqual(prog('ar'), env('AR'))
        self.assertEqual(prog('cpp'), env('CPP'))
        self.assertEqual(prog('gcc'), env('CC'))
        self.assertEqual(prog('g++'), env('CXX'))
        self.assertEqual(prog('objcopy'), env('OBJCOPY'))
        self.assertEqual(prog('ranlib'), env('RANLIB'))
        self.assertEqual(prog('g++'), env('LINK_CXX'))

    def testCompilerFlags(self):
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=DKAWithCompiler, build=DKABuild)
        self.suite.run_waf()
        self.assertTrue('CXXFLAGS' in Build.bld.env)
        self.assertTrue('CCFLAGS' in Build.bld.env)
        self.assertTrue('CCDEFINES' in Build.bld.env)
        self.assertTrue('CXXDEFINES' in Build.bld.env)
        self.assertTrue('CPPPATH' in Build.bld.env)
        self.assertTrue('LIBPATH' in Build.bld.env)
        self.assertTrue('LINKFLAGS' in Build.bld.env)
        self.assertEqual(list, type(Build.bld.env['CPPPATH']))
        self.assertEqual(list, type(Build.bld.env['LIBPATH']))
        self.assertEqual(list, type(Build.bld.env['LINKFLAGS']))
        self.assertEqual(5, len(Build.bld.env['LINKFLAGS']))

    def testCheckLibnds(self):
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=DKAWithLibnds, build=DKABuild)
        self.suite.run_waf()
        self.assertEquals(['fat', 'nds9'], Build.bld.env['LIB_NDS'])

if __name__ == '__main__':
    unittest.main()
