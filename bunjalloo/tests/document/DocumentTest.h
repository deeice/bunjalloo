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
#ifndef DocumentTest_h_seen
#define DocumentTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class Document;
class DocumentTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DocumentTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST( testHtmlAttributes );
  CPPUNIT_TEST( testHead );
  CPPUNIT_TEST( testEmpty );
  CPPUNIT_TEST( testHead2 );
  CPPUNIT_TEST( testTitle );
  CPPUNIT_TEST( testHead3 );
  CPPUNIT_TEST( testGoogle );
  CPPUNIT_TEST( testAnchor );
  CPPUNIT_TEST( testBrokenAnchor );
  CPPUNIT_TEST( testCharacterStart );
  CPPUNIT_TEST( testEndTagStart );
  CPPUNIT_TEST( testMismatchFormat );
  CPPUNIT_TEST( testSimpleBodyA );
  CPPUNIT_TEST( testLi );
  CPPUNIT_TEST( testDD );
  CPPUNIT_TEST( testPlaintext );
  CPPUNIT_TEST( testFont );
  CPPUNIT_TEST( testFont2);
  CPPUNIT_TEST( testPin8);
  CPPUNIT_TEST( testClarin);
  CPPUNIT_TEST( testAdoption);
  CPPUNIT_TEST( testAdoption2);
  CPPUNIT_TEST( testHeader);
  CPPUNIT_TEST( testHeader2);
  CPPUNIT_TEST( testAttribs);
  CPPUNIT_TEST( testUnicode2String);
  CPPUNIT_TEST( testActiveFormatters);
  CPPUNIT_TEST( testHistory);
  CPPUNIT_TEST_SUITE_END();

  public:
  void setUp();
  void tearDown();
  void test0();
  void test1();
  void testHtmlAttributes();
  void testHead();
  void testEmpty();
  void testHead2();
  void testTitle();
  void testHead3();
  void testGoogle();
  void testAnchor();
  void testBrokenAnchor();
  void testCharacterStart();
  void testEndTagStart();
  void testMismatchFormat();
  void testSimpleBodyA();
  void testLi();
  void testDD();
  void testPlaintext();
  void testFont();
  void testFont2();
  void testPin8();
  void testClarin();
  void testAdoption();
  void testAdoption2();
  void testHeader();
  void testHeader2();
  void testAttribs();
  void testUnicode2String();
  void testActiveFormatters();
  void testHistory();

  private:
  Document * m_document;
  char * m_data;
  unsigned int m_length;
  void readFile(const char * fileName);
};

#endif
