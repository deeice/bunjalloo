import os
import unittest
import waftester

def MyBuild(bld):
    pass

def MyConf(bld):
    pass

def getcwd():
    return os.path.abspath(os.path.dirname(__file__))

class TestWafTest(waftester.WafTestCase):

    def testSubdir(self):
        self.suite.subdir('source')
        self.assertTrue(os.path.exists(self.suite.basedir + '/source'))

    def testWriteData(self):
        data = "a\nb\nc\n"
        self.suite.write_data(data, 'test.txt')
        self.CompareContents(data, self.suite.basedir+'/test.txt')

    def testWriteConfigure(self):
        self.suite.write_wscript(configure=MyConf)
        cwd = getcwd()
        self.CompareContents("""\
import sys
sys.path.append("%s")
def configure(ctx):
    from test_waftester import MyConf
    MyConf(ctx)
""" % cwd, self.suite.basedir + "/wscript")

    def testWriteBuild(self):
        self.suite.write_wscript(build=MyBuild)
        self.CompareContents("""\
import sys
sys.path.append("%s")
def build(ctx):
    from test_waftester import MyBuild
    MyBuild(ctx)
""" % getcwd(), self.suite.basedir + "/wscript")

    def testWriteWscript(self):
        self.suite.write_wscript(configure=MyConf, build=MyBuild)
        cwd = getcwd()
        self.CompareContents("""\
import sys
sys.path.append("%s")
def configure(ctx):
    from test_waftester import MyConf
    MyConf(ctx)
sys.path.append("%s")
def build(ctx):
    from test_waftester import MyBuild
    MyBuild(ctx)
""" % (cwd,cwd), self.suite.basedir + "/wscript")


    def testWafRun(self):
        self.suite.write_wscript(configure=MyConf, build=MyBuild)
        self.suite.run_waf()

if __name__ == '__main__':
    unittest.main()
