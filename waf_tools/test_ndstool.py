import unittest
import waftester
import Build
import base64
import scwaf

def NdsDetect(conf):
    conf.check_tool('ndstool')

def FullNdsDetect(conf):
    conf.check_tool('devkitarm')
    conf.check_tool('compiler_cxx')
    NdsDetect(conf)
    conf.check_libnds()

def NdsBuild(bld):
    bld.new_task_gen(features='cxx cprogram', source='main.cpp', target='test', uselib='NDS')
    bld.create_nds('test.nds', 'test')

def NdsBuildBannerNoIcon(bld):
    bld.new_task_gen(features='cxx cprogram', source='main.cpp', target='test', uselib='NDS')
    bld.create_nds('test.nds', 'test', banner='this;is;banner')

def NdsBuildBanner(bld):
    bld.new_task_gen(features='cxx cprogram', source='main.cpp', target='test', uselib='NDS')
    bld.create_nds('test.nds', 'test', banner='this;is;banner', icon='icon.bmp')

main_cpp_code = """
int main()
{
}
"""
icon_data = """
Qk12AgAAAAAAAHYAAAAoAAAAIAAAACAAAAABAAQAAAAAAAACAAATCwAAEwsAABAAAAAQAAAA/wD/
APaLRwASitEA93kuAACX7AATmesAJar3ADus7QCky+EArd73AN/i3wDv594A++rcAMzr/gD++PEA
+//9ADMRERERERERERERERERETMxERERERERERERERERERETERERoaoREREfoREREREREREzH//6
oRER+hERERERMxETM/////8RHqERERMzMzMxMzMz//8zMx+hETMzMzMzMzMzMz8zMzMz+jMzMzMz
MzMzMzMzMzMzMz+jMzMzMzMzMzMzMzMzMzM/ozMzMzMzMzMzMzMzMzMz6hMzMzqqMzMzMzMzMzMz
M/ozMz//+qMzMzMzMzMzMzP6MzMz///6MzMzMzMzM/8+/zMzMz//MzMzMzMzM//6+7vDMzMzMzMz
MzMzMz/6r7IivDMzMzMzMzMzMzMzOstkRCvDMzMzMzMzMzMzMzy2RVVCvDMzMzMzozMzMzPMdFVV
VCzDMzMzM6qjMzM8x03//9dCzDMzMzP//zMzzHRZlVbdVCzDMzMz/zMzPKdFWZVVn1VCrDMzM/Mz
Mz+kVVmVVd1VUq8zMzMzMzM/plVZ///3VVWvMzMzMzMzP6dVWZVX+VVVrzMzMzMzMz+kZVmVVd9V
VK8zOqozMzMx+EZZlVf5VUjxM//6MzMz+q6Gbf//lVSOEz///zMzP//66EZVVVVI4TMzP/MzMzP/
/x6EZmZkjhMzMzMzMzMzP/Mx6IiIiOEzMzMzMzMzMzMzMx7u7u4TMzMzMzMzMzMzMzMzMzMzMzMz
MzMz
"""

class NdsToolTest(waftester.WafTestCase):

    def testNdsDetect(self):
        # requires grit to be installed
        self.suite.write_wscript(configure=NdsDetect, build=waftester.DoNothing)
        self.suite.run_waf()
        self.assertTrue(Build.bld.env['NDSTOOL'])

    def testNdsBuild(self):
        self.suite.write_data(main_cpp_code, 'main.cpp')
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=FullNdsDetect, build=NdsBuild)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('test.nds'))

    def testNdsBuildBannerNoIcon(self):
        self.suite.write_data(main_cpp_code, 'main.cpp')
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=FullNdsDetect, build=NdsBuildBannerNoIcon)
        self.assertRaises(waftester.WafTestException, self.suite.run_waf)

    def testNdsBuildBanner(self):
        self.suite.write_data(main_cpp_code, 'main.cpp')
        self.suite.write_data(base64.decodestring(icon_data), 'icon.bmp')
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=FullNdsDetect, build=NdsBuildBanner)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('test.nds'))
        self.assertTrue(self.suite.exists('test.arm'))

if __name__ == '__main__':
    unittest.main()
