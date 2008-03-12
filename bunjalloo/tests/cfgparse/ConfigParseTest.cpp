/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "Config.h"
#include "ConfigParser.h"
#include "CookieJar.h"

using namespace std;

class ConfigParseTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ConfigParseTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST_SUITE_END();

  public:
  void test0();
  void test1();
  void test2();
  void setUp();
  void tearDown();

  private:
  CookieJar * m_cookieJar;
  Config * m_config;
  ConfigParser * m_configParser;


};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ConfigParseTest );

void ConfigParseTest::setUp()
{
  m_cookieJar = new CookieJar;
  m_config = new Config(*m_cookieJar);
  m_configParser = new ConfigParser(*m_config);
}

void ConfigParseTest::tearDown()
{
  delete m_cookieJar;
  delete m_config;
  delete m_configParser;
}

void ConfigParseTest::test0()
{
  string expected("Hello");
  string line("%Hello%");
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

  expected = "";
  line = "%%";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

  expected = "This is empty: end";
  line = "This is empty:%% end";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

  expected = "This is full";
  line = "This is %full%";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

  expected = "This is has a percent% sign";
  line = "This is has a percent% sign";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);
}

void ConfigParseTest::test1()
{
  string expected = "This has two values";
  string line = "This has %two% %values%";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);
}


void ConfigParseTest::test2()
{
  string expected("selected=1");
  string line("%language=nl?selected=1%");
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

  expected = "";
  line = "%%";
  m_configParser->replaceMarkers(line);
  CPPUNIT_ASSERT_EQUAL( expected , line);

}
