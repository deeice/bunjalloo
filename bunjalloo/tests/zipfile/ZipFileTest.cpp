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
#include <cppunit/extensions/HelperMacros.h>
#include "ZipFile.h"

using namespace std;

class ZipFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ZipFileTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST_SUITE_END();

  public:
  void test0();
  void setUp();
  void tearDown();

  private:
  ZipFile * m_zipfile;


};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ZipFileTest );

void ZipFileTest::setUp()
{
  m_zipfile = new ZipFile;
}

void ZipFileTest::tearDown()
{
  delete m_zipfile;
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

}
