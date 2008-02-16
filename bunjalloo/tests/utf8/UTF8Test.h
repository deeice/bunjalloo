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
#ifndef UTF8Test_h_seen
#define UTF8Test_h_seen

#include <cppunit/extensions/HelperMacros.h>

class UTF8Test : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( UTF8Test );
  CPPUNIT_TEST( testZero );
  CPPUNIT_TEST( test80 );
  CPPUNIT_TEST( test800 );
  CPPUNIT_TEST( test1000 );
  CPPUNIT_TEST( test20000 );
  CPPUNIT_TEST( test4000000 );

  CPPUNIT_TEST( test7f );
  CPPUNIT_TEST( test7ff );
  CPPUNIT_TEST( testffff );
  CPPUNIT_TEST( test1fffff );
  CPPUNIT_TEST( test3ffffff );
  CPPUNIT_TEST( test7fffffff );

  CPPUNIT_TEST( testD7FF );
  CPPUNIT_TEST( testE000 );
  CPPUNIT_TEST( testFFFD );
  CPPUNIT_TEST( test10FFFF );
  CPPUNIT_TEST( test110000 );

  CPPUNIT_TEST( testFirstContinuation );
  CPPUNIT_TEST( testLastContinuation );
  CPPUNIT_TEST( test2Continuations );
  CPPUNIT_TEST( testLonely2 );
  CPPUNIT_TEST( testLonely3 );
  CPPUNIT_TEST( testLonely4 );
  CPPUNIT_TEST( testLonely5 );
  CPPUNIT_TEST( testLonely6 );
  CPPUNIT_TEST( testOverlong );
  CPPUNIT_TEST( testInvalid );

  CPPUNIT_TEST( testCopyright );
  CPPUNIT_TEST( testSymbolNE );

  CPPUNIT_TEST( testEncode0 );
  CPPUNIT_TEST_SUITE_END();

public:
  void testZero();
  void test80();
  void test800();
  void test1000();
  void test20000();
  void test4000000();
  void test7f();
  void test7ff();
  void testffff();
  void test1fffff();
  void test3ffffff();
  void test7fffffff();
  void testD7FF();
  void testE000();
  void testFFFD();
  void test10FFFF();
  void test110000();

  void testFirstContinuation();
  void testLastContinuation();
  void test2Continuations();
  void testLonely2();
  void testLonely3();
  void testLonely4();
  void testLonely5();
  void testLonely6();

  void testOverlong();
  void testInvalid();
  void testCopyright();
  void testSymbolNE();

  void testEncode0();
};

#endif
