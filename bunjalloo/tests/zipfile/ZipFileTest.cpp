/*
  Copyright (C) 2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <unistd.h>
#include <cppunit/extensions/HelperMacros.h>
#include "File.h"
#include "ZipFile.h"

using namespace std;

class ZipFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ZipFileTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST_SUITE_END();

  public:
  void test0();
  void test1();
  void setUp();
  void tearDown();

  private:
  ZipFile * m_zipfile;
  bool m_hasTestDir;
  void cdTestDir();


};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ZipFileTest );

void ZipFileTest::setUp()
{
  m_zipfile = new ZipFile;
  m_hasTestDir = false;
}
void ZipFileTest::cdTestDir()
{
  nds::File::mkdir("test");
  chdir("test");
  m_hasTestDir = true;
}
void ZipFileTest::tearDown()
{
  if (m_hasTestDir)
  {
    chdir("..");
    nds::File::rmrf("test");
  }
  delete m_zipfile;
}

static std::string readFile(const char * filename)
{
  nds::File f;
  f.open(filename);
  if (f.is_open())
  {
    int size = f.size();
    char * data = new char[size+1];
    f.read(data);
    data[size] = 0;
    string s(data);
    delete [] data;
    return s;
  }
  return "";
}

void ZipFileTest::test0()
{
  string filename("data/test0.zip");
  m_zipfile->open(filename.c_str());
  //CPPUNIT_ASSERT_EQUAL( expected , line);
  CPPUNIT_ASSERT( m_zipfile->is_open());
  vector<string> files;
  m_zipfile->list(files);
  size_t expected(2);
  CPPUNIT_ASSERT_EQUAL(expected, files.size());

  string expected1("file1.txt");
  CPPUNIT_ASSERT_EQUAL(expected1, files[0]);
  string expected2("file2.txt");
  CPPUNIT_ASSERT_EQUAL(expected2, files[1]);

  cdTestDir();
  // now unzip and check the files exists
  m_zipfile->extract();
  nds::File::FileType expectedType(nds::File::F_REG);
  CPPUNIT_ASSERT_EQUAL(expectedType, nds::File::exists("file1.txt"));
  CPPUNIT_ASSERT_EQUAL(expectedType, nds::File::exists("file2.txt"));
  expectedType = nds::File::F_NONE;
  CPPUNIT_ASSERT_EQUAL(expectedType, nds::File::exists("test0.zip"));

  string contents = readFile("file1.txt");
  string expectedContents("Wed Mar 19 19:54:44 CET 2008\n");
  CPPUNIT_ASSERT_EQUAL(expectedContents, contents);

  contents = readFile("file2.txt");
  expectedContents = "Linux rich-laptop 2.6.22-14-generic #1 SMP Tue Feb 12 07:42:25 UTC 2008 i686 GNU/Linux\n";
  CPPUNIT_ASSERT_EQUAL(expectedContents, contents);
}

void ZipFileTest::test1()
{
  string filename("/data/test0.zip");
  m_zipfile->open(filename.c_str());
  //CPPUNIT_ASSERT_EQUAL( expected , line);
  CPPUNIT_ASSERT( m_zipfile->is_open());

  cdTestDir();
  // now unzip and check the files exists
  m_zipfile->extract();
  nds::File::FileType expectedType(nds::File::F_REG);
  CPPUNIT_ASSERT_EQUAL(expectedType, nds::File::exists("file1.txt"));
  CPPUNIT_ASSERT_EQUAL(expectedType, nds::File::exists("file2.txt"));
}
