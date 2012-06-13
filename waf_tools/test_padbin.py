import waftester
import unittest
import Build

def PadbinConf(conf):
    conf.check_tool('padbin')

def PadbinBuild(bld):
    o = bld.new_task_gen(features='padbin', source='in.bin', target='out.bin')
    o.env['PADBINFLAGS'] = '4'


class PadbinTest(waftester.WafTestCase):
    def testDetect(self):
        self.suite.write_wscript(configure=PadbinConf, build=waftester.DoNothing)
        self.suite.run_waf()
        self.assertTrue(Build.bld.env['PADBIN'])

    def testDoesPadding(self):
        self.suite.write_wscript(configure=PadbinConf, build=PadbinBuild)
        self.suite.write_data('abc', 'in.bin')
        self.suite.run_waf()
        self.assertTrue(self.suite.exists('out.bin'))
        f = self.suite.get_file('out.bin')
        f.seek(0, 2) # EOF
        self.assertEqual(4, f.tell())
        f.close()

if __name__ == '__main__':
    unittest.main()
