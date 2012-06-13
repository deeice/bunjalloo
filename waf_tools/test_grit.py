import unittest
import sys
import os
import waftester
import Build
import base64
import scwaf
import grit

# base64 encoded image data
image_data = """\
iVBORw0KGgoAAAANSUhEUgAAAwAAAAAICAQAAADkM1D1AAAAAXNSR0IArs4c6QAAAAJiS0dEAP+H
j8y/AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH2AUECx4wi/bMVwAAAz1JREFUaN7lWsl2
4yAQpPL8/79cc4lj0XsDSjyJcnAkEDS9FoVADvPCoLi7PpH3sr8ebYh23V+OP8L+8XzV6zkKxhAS
ytXN7Vb/1z2+ntLsn7Vb49fXUmnJ5rPlrbbL9eX6oJDT1u8Q/aBGsP/Pxz+zfk+++TeXT/9G+rPs
nvlz3Xc60Rb1QuDP2PZ0tKPlf78Q2Lejjcc5cTAFvHRstIzKQnCfTP92gsj6zTJikj96X86Sj28Z
vB7gXrGu/o5mu3a/WB+5xNaTalm34Yx8yo3187hn2tqzPczSDEVvJPpjAnjORFpHRz3Al+eX+xMy
32a2jq4/6i74ckN8pXxcxKFrMs9FowXxgrtwa/qPQq6Szupy8TaXmUvF9c8KEJojYDkx7Sa20/bj
p3fy5gTBY+NAxUsnBWare8YnymvhoOgPdYfAf2D6E1U7XP9jqmsEUBTTiEjks9tH+f2h1jKMe/1c
PoH7HEvxWYntx5rbz4hCO/BICKOdAEGCUlAK0j6lhACBo0kx2OmJQUBHASF3TzVMbWFsL7i6iNBe
f4VEWdsA56vdG9G2Dxv+y8t+AeKJlpcLCDiHM137eajSI2j9Ha29j5e7/GivX9UHp7FepY/hDs5q
96Wx4epefmNY4KP9bOYRWh+bBQAG6SOX8KREKPDHmpLmgKHjEEMQLD30yOYbVJSOJkB8iiSjJGhi
Ba8AUZFFFn2Qn8H4Lsey41XxG0d0VlCxVr4N7qBkFsNpTR9VD18nGHhYmpWy7IMwpPp9Z8JlmFDp
5P6VrVMYtuAqQ5kffZPoHQDC9FYb009QMZW0gvCw6XBnjsxOvs+Af68VPH6LnGeDsjf/Hg3Egn9X
ZbXgUrSe2nz8YXv0yundtudGfsjl651oMP2IBoaHnI409iigzpQc+kDN2jDXj6H0aJUSspq2aFIW
c4GDqqYakcNpzyiDSv8M0bL1FVBMQfUpkJzyifVtzScpRqYBPMpnMT17SA2vvb8HCSwKjcG2fze9
SmoEof2f7fNpQ0ZKnSrI6+vzMX4kHUq5yKP4qAq+BQBmSk3am8lXVXXfA3/J11P4c5+B/a31/oSG
f+enhbUzsu+QgW9jiQwivbd+dq5/nyUuAtkfArEAAAAASUVORK5CYII=
"""

pcx_image= '''\
CgUBCAAAAAAPAE8AQAHIAOAAgAD/AACAAP////8AAAC7AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAABEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAADQANAA0ADQANAA0ADQANAAzAACAcIAzgABAMQAAgEAAQIBAsMBAgDDAAEE
yAHDAMIAxgHCAAXCAAHCAMIAAgEABQABxQABwgDDAALDAAHIANAA0ADQANAA0ADQANAA0ADQAMwA
AgHCAM4AAQDDAAECAQABAgECwwECAADDAQTIAcMAAALCA8QBwgAFwgABwgDCAMIBAAUAAcUAAcIA
wgACxAAByADQANAA0ADQANAA0ADQANAA0ADMAAIBwgAAwgHLAAEAAMIDAQIBAAECAQLDAQIAwwAD
BMgBwwDEAMQBwgAFwgABwgDCAAMBAgUAAcUAAcIAwgDCAsMAAcgA0ADQANAA0ADQANAA0ADQANAA
zAACAcIAzgABAMMAAQIBAAECAQLDAQIAAMMBBMgBwwAAAsIDxAHCAAXCAAHCAMIAwgEABQABxQAB
wgDCAAXEAAHIANAA0ADQANAA0ADQANAA0ADQANAAywABxADCAAHJAMIBwgDDAAEFwgABwgABwgAB
BQDEAMkBBQIAAMUBAsIBAgEFAgXCANAA0AAM4ACAAP8AAIAA/////wAAALsAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
'''

def GritDetect(conf):
    conf.env['GRITFLAGS'] = ['-ftc', '-m!']
    conf.check_tool('grit', '.')

def GritDetect2(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')
    conf.env['GRITFLAGS'] = '-pw 16 -gB 8 -m! -ft c -fh! -q'.split()
    conf.check_tool('grit')

def GritDetectWithHeader(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')
    conf.check_tool('grit')

def GritBuild(bld):
    obj = bld.transform_image('input.png',
            GRITFLAGS='-pw 16 -gB 8 -m! -ftc -fh -q'.split())

def GritBuild2(bld):
    obj = bld.transform_image('input.png',
            GRITFLAGS = '-pw 16 -gB 8 -m! -ft c -fh -q'.split())

def GritBuildAndCompile(bld):
    bld.transform_image('images/input.png')
    bld.new_task_gen(features='cc', source='images/input.c', target="img_obj")
    bld.new_task_gen(features='cc cstaticlib', target='img', add_objects='img_obj')

class GritTest(waftester.WafTestCase):

    def testGetExt(self):
        self.assertEqual(sorted(['.c', '.h']), sorted(grit.grit_ext('-ftc -fh')))
        self.assertEqual(sorted(['.c', '.h']), sorted(grit.grit_ext('-m -ftc -pw 16')))
        self.assertEqual(sorted(['.c', '.h']), sorted(grit.grit_ext('-m -ft c -pw 16')))
        self.assertEqual(sorted(['.c']), sorted(grit.grit_ext('-m -ft c -pw 16 -fh!')))
        self.assertEqual(sorted(['.img.bin']), sorted(grit.grit_ext('-m -ft b -pw 16 -fh!')))

    def testGritDetect(self):
        # requires grit to be installed
        self.suite.write_wscript(configure=GritDetect, build=waftester.DoNothing)
        self.suite.run_waf()
        self.assertTrue(Build.bld.env['GRIT'])
        self.assertEquals(['-ftc', '-m!'], Build.bld.env['GRITFLAGS'])

    def testGritSingle(self):
        """
        check that grit is capable of running:
            png -> .c
        """
        self.suite.write_data(base64.decodestring(image_data), 'input.png')
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=GritDetect2, build=GritBuild)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('input.c'), "input.c must have been created")

    def testGritSingleCompile(self):
        """
        check that grit is capable of running:
            png -> .c -> .o -> lib
        """
        self.suite.subdir('images')
        self.suite.write_data(base64.decodestring(image_data), 'images/input.png')
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=GritDetect2, build=GritBuildAndCompile)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('images/input.c'), "input.c must have been created")

        staticlib_PATTERN = Build.bld.env['staticlib_PATTERN']
        obj_PATTERN = '%s_2.o' # 2nd task gen..
        self.assertTrue(self.suite.exists(obj_PATTERN % 'images/input'), "object not created")
        self.assertTrue(self.suite.exists(staticlib_PATTERN % 'img'), "img library must be created")

    def testDevkitARMUnset(self):
        dka = os.environ.pop('DEVKITARM')
        try:
            self.suite.write_wscript(configure=GritDetect, build=waftester.DoNothing)
            self.assertRaises(waftester.WafTestException, self.suite.run_waf)
        finally:
            os.environ['DEVKITARM'] = dka

    def testCleanRemovesHeader(self):
        self.suite.write_data(base64.decodestring(image_data), 'input.png')
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=GritDetectWithHeader, build=GritBuild)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('input.h'), "input.h must be created")
        self.assertTrue(self.suite.exists('input.c'), "input.c must be created")
        self.suite.run_waf(['clean'])
        self.assertFalse(self.suite.exists('input.h'), "input.h must be removed on clean")
        self.assertFalse(self.suite.exists('input.c'), "input.c must be removed on clean")

    def testCleanRemovesHeader2(self):
        self.suite.write_data(base64.decodestring(image_data), 'input.png')
        self.suite.write_wscript(options=waftester.CompilerOptions, configure=GritDetectWithHeader, build=GritBuild2)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('input.h'), "input.h must be created")
        self.assertTrue(self.suite.exists('input.c'), "input.c must be created")
        self.suite.run_waf(['clean'])
        self.assertFalse(self.suite.exists('input.h'), "input.h must be removed on clean")
        self.assertFalse(self.suite.exists('input.c'), "input.c must be removed on clean")

def GritBuildAndCompileMulti(bld):
    bld.transform_image(bld.glob('input_*.png'),
            GRITFLAGS='-pw 16 -gB 8 -m! -ftc -fh -q'.split())
    bld.new_task_gen(features='cxx', source=bld.glob('input_*.c'), target="img_obj")
    bld.static_library('img', ".", add_objects = 'img_obj')

def GritBuildPal(bld):
    bld.transform_image(bld.glob('images/input_*.png'),
            GRITFLAGS='-pw 16 -gB 8 -m! -ftc -fh -q -pS -Opalette0'.split())
    source = bld.glob('images/*.png')
    source += ['images/palette0.c']
    o = bld.new_task_gen(features='cc', target="img_obj",
            source=[s.replace('.png', '.c') for s in source])
    bld.static_library('img', ".", add_objects = 'img_obj', export_incdirs='images')

class GritTestMulti(waftester.WafTestCase):
    def testMulti(self):
        """ Test to check that multiple inputs are converted """
        for i in xrange(0, 4):
            self.suite.write_data(base64.decodestring(image_data), 'input_%d.png'%i)
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=GritDetectWithHeader, build=GritBuildAndCompileMulti)
        self.suite.run_waf()
        for i in xrange(0, 4):
            self.assertTrue(self.suite.exists('input_%d.h'%i), 'input_%d.h not created'%i)
            self.assertTrue(self.suite.exists('input_%d.c'%i), 'input_%d.c not created'%i)

    def testGetPalette(self):
        self.assertEquals('foo.c', grit.get_palette('-q -ftc -pS -Ofoo.c'))
        self.assertEquals(None, grit.get_palette('-pw 16 -gB 8 -m!'))
        self.assertEquals('foo', grit.get_palette('-q -ftc -pS -Ofoo'))

    def testMultiPal(self):
        self.suite.subdir('images')
        for i in xrange(0, 4):
            self.suite.write_data(base64.decodestring(image_data), 'images/input_%d.png'%i)
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=GritDetectWithHeader, build=GritBuildPal)
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('images/palette0.c'), 'palette.c not created')
        self.assertTrue(self.suite.exists('images/input_0_2.o'), 'input_0 not created')

def GritSub0(bld):
    bld.add_subdirs('sub')

def GritSub(bld):
    palettes = ['palette' + hex(i)[2] for i in xrange(0, 2)]
    for i in palettes:
        bld.transform_image(bld.glob(i+'/input_*.pcx'),
            GRITFLAGS=('-pw 16 -gB 8 -m! -ftc -fh -q -pS -O'+i).split())
    source = bld.glob('palette*/*.pcx')
    source += ['/'.join((p, p+'.c')) for p in palettes]
    bld.new_task_gen(features='cc', target="img_obj",
            source=[s.replace('.pcx', '.c') for s in source])
    pal = bld.static_library('pal', ".", add_objects = 'img_obj', export_incdirs=palettes)
    pal.include_directories(palettes)
    tm = bld.task_manager
    for i in sorted(tm.groups[0].tasks_gen):
        print i


class GritTestSubdir(waftester.WafTestCase):
    def testSubdirs(self):
        self.suite.subdir('sub')
        self.suite.subdir('sub/palette0')
        self.suite.subdir('sub/palette1')
        for d in xrange(0, 2):
            for i in xrange(0, 4):
                self.suite.write_data(base64.decodestring(pcx_image), 'sub/palette%d/input_%d.pcx'%(d, i))
        self.suite.write_wscript(options=waftester.CompilerOptions,
                configure=GritDetectWithHeader, build=GritSub0)
        self.suite.write_wscript(filename='sub/wscript', build=GritSub)
        self.suite.run_waf()


if __name__ == '__main__':
    unittest.main()
