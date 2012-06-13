#!/usr/bin/python
import unittest
import os
import waftester
import Build

def ConfigureObjcopy(conf):
    conf.check_tool('misc')
    conf.check_tool('objcopy')
    conf.env['OBJCOPYFLAGS'] = '-I binary -O elf32-i386 -B i386'.split()

def BuildObjcopy(bld):
    bld.new_task_gen(features='copy', source='dir/data.txt.in', target='dir/data.txt')
    bld.objcopy('dir/data.txt')

def CompileWithName2h(bld):
    datas = bld.glob('dir/data*.in')
    targets = []
    for f in datas:
        newf = f.replace('.in', '')
        bld.new_task_gen(features='copy', source=f, target=newf)
        bld.objcopy(newf)
        targets.append(newf)
    bld.new_task_gen(features='name2h', source=targets, target='dir/data.h')

expected_contents = """\
#ifndef data_h_seen
#define data_h_seen
#ifdef __cplusplus
extern "C" {
#endif

extern const u16 _binary_data_0_txt_end[];
extern const u16 _binary_data_0_txt_start[];
extern const u32 _binary_data_0_txt_size[];

extern const u16 _binary_data_1_txt_end[];
extern const u16 _binary_data_1_txt_start[];
extern const u32 _binary_data_1_txt_size[];

#ifdef __cplusplus
};
#endif
#endif
"""

class ObjcopyTest(waftester.WafTestCase):
    def testObjcopyDetect(self):
        self.suite.write_wscript(configure=ConfigureObjcopy, build=waftester.DoNothing)
        self.suite.run_waf()
        self.assertTrue(Build.bld.env['OBJCOPY'])

    def testObjcopyDetectEnv(self):
        try:
            self.suite.write_wscript(configure=ConfigureObjcopy, build=waftester.DoNothing)
            os.environ['OBJCOPY'] = 'some-random-objcopy'
            self.suite.run_waf()
            self.assertEquals('some-random-objcopy', Build.bld.env['OBJCOPY'])
        finally:
            os.environ.pop('OBJCOPY')

    def testObjcopyDoesCopy(self):
        self.suite.subdir('dir')
        self.suite.write_data('some data\n', 'dir/data.txt.in')
        self.suite.write_wscript(build=BuildObjcopy, configure=ConfigureObjcopy)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('dir/data.o'))

    def testName2h(self):
        self.suite.subdir('dir')
        for d in xrange(0, 2):
            self.suite.write_data('some data\n', 'dir/data_%d.txt.in'%d)
        self.suite.write_wscript(build=CompileWithName2h, configure=ConfigureObjcopy)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('dir/data.h'))
        f = self.suite.get_file('dir/data.h')
        self.CompareContents(expected_contents, f)


if __name__ == '__main__':
    unittest.main()
