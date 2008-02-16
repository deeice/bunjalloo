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
#ifndef HtmlElementTest_h_seen
#define HtmlElementTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class HtmlElement;
class HtmlElementTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( HtmlElementTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testAttributes );
  CPPUNIT_TEST( testAppend );
  CPPUNIT_TEST( testIsa );
  CPPUNIT_TEST( testAnchor );
  CPPUNIT_TEST( testClone );
  CPPUNIT_TEST( testParent );
  CPPUNIT_TEST( testAppendText );
  CPPUNIT_TEST( testMeta );
  CPPUNIT_TEST( testRemove );
  CPPUNIT_TEST( testImg );
  CPPUNIT_TEST( testPreviousSibling );
  CPPUNIT_TEST( testNextSibling );
  CPPUNIT_TEST( testOptionElement );
  CPPUNIT_TEST( testElementsByTagName );
  CPPUNIT_TEST( testTextAreaElement );
  CPPUNIT_TEST( testInputElement );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test0();
  void testAttributes();
  void testAppend();
  void testIsa();
  void testAnchor();
  void testClone();

  void testParent();
  void testAppendText();
  void testMeta();
  void testRemove();
  void testImg();
  void testPreviousSibling();
  void testNextSibling();
  void testOptionElement();
  void testElementsByTagName();
  void testTextAreaElement();
  void testInputElement();

private:
  HtmlElement * m_element;
  HtmlElement * m_clone;
};

#endif
