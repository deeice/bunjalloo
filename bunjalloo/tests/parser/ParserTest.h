/*
  Copyright (C) 2007 Richard Quirk

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
#ifndef ParserTest_h_seen
#define ParserTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class HtmlParserDriver;
class HeaderParser;
class ParserTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ParserTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testRedirect );
  CPPUNIT_TEST( testNotFound );
  CPPUNIT_TEST( testNotHtml );
  CPPUNIT_TEST( testIso );
  CPPUNIT_TEST( testRefresh );
  CPPUNIT_TEST( testBogusDoctype );
  CPPUNIT_TEST( testMetaIso );
  CPPUNIT_TEST( testSlashdot );
  CPPUNIT_TEST( testNumbers );
  CPPUNIT_TEST( testGzip );
  CPPUNIT_TEST( testNewline );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test0();
  void testRedirect();
  void testNotFound();
  void testNotHtml();
  void testIso();
  void testRefresh();
  void testBogusDoctype();
  void testMetaIso();
  void testSlashdot();
  void testNumbers();
  void testGzip();
  void testNewline();

  void readFile(const char * fileName);
private:
  HtmlParserDriver * m_htmlParser;
  HeaderParser * m_headerParser;
  char * m_data;
  unsigned int m_length;
};

#endif
