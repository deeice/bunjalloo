import unittest
import waftester
import Build
import sox
import base64

test_wav = """
UklGRqMBAABXQVZFZm10IBIAAAABAAEAgD4AAIA+AAABAAgAAABkYXRhfQEAAN7e3t7e3t7e3t7e
3t7e3t7e3t7bp9KnvK2zvLahxbbSqqHSs7+qsLOq597ng+Hn3uF83nnb24B5ht7b5IC/5+HngOfh
29580nbb24Z8mOHe54PV597kgOeA29t8fHzb296AquTe54DkfNjh3t7blYChpOHh3t7bgIB8gOTe
3t7eeYCScOHh3t7ng4C2ed7e3uHneYPbkuHe3uHkfIbkc97e3t7ng4DOht7e3uHehobkqt7e3uHI
gIPeyOHe4d6ng3y83t7h3tvSg3mSvN7e3t62gIbkeefe3t7e3uHVg4B53pXb3t7e3t7bgICDiYDk
4eHe3uHh4YOAhuSJ297e3t7e24aAg4CJ2+Hh3t7h4eeDg4bhgN7h3t7e3tuVgIB5mNLb3t7e3t7n
gICD1Xnh4d7e3uHepICAea3C297e3t7e5HyAgMJ22OHe3t7e4bmAg3y/fN7e3t7e3uF5gICqdsvh
3t7e3uHLgIOA5+He3t7e3t7e3t7e3t7e3t7e3g==
"""

def SoxDetect(conf):
    conf.check_tool('sox')

class TestSox(waftester.WafTestCase):
    def testSoxDetect(self):
        # requires grit to be installed
        self.suite.write_wscript(configure=SoxDetect, build=waftester.DoNothing)
        self.suite.run_waf()
        self.assertTrue(Build.bld.env['SOX'])

    def testFlagParsingOld(self):
        have_one, have_wav = sox.parse_help('''
        fopts: -b/-w/-l/-d -v volume -x

        Supported file formats: vorbis vox wav wve
        '''.split('\n'))
        self.assertFalse(have_one)
        self.assertTrue(have_wav)

    def testFlagParsingNewerNoWav(self):
        have_one, have_wav = sox.parse_help('''
        fopts: -1/-w/-l/-d -v volume -x

        Supported file formats: wve
        '''.split('\n'))
        self.assertTrue(have_one)
        self.assertFalse(have_wav)

    def testFlagParsingNewer(self):
        have_one, have_wav = sox.parse_help('''
        -1/-2/-3/-4/-8           Encoded sample size in bytes

        AUDIO FILE FORMATS: wav wve
        '''.split('\n'))
        self.assertTrue(have_one)
        self.assertTrue(have_wav)

    def testGetHelp(self):
        self.assertNotEqual("", sox.get_help('sox'))

def SoxBuild(bld):
    bld.new_task_gen(source='test.wav')

def SoxBuild(bld):
    bld.new_task_gen(source=bld.glob('*.wav'))

class TestSoxBuild(waftester.WafTestCase):
    def testConvertWav(self):
        """ Test converting a wav to a raw """
        self.suite.write_wscript(configure=SoxDetect, build=SoxBuild)
        self.suite.write_data(base64.decodestring(test_wav), 'test.wav')
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('test.raw'))

    def testConvertMultiWav(self):
        self.suite.write_wscript(configure=SoxDetect, build=SoxBuild)
        for i in xrange(0, 10):
            self.suite.write_data(base64.decodestring(test_wav), 'test_%d.wav'%i)
        self.suite.run_waf()
        for i in xrange(0, 10):
            self.assertTrue(self.suite.exists('test_%d.raw'%i))

if __name__ == '__main__':
    unittest.main()
