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
#ifndef UriTest_h_seen
#define UriTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class UriTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( UriTest );
    CPPUNIT_TEST( testSimpleHttp );
    CPPUNIT_TEST( testBroken );
    CPPUNIT_TEST( testNavigate );
    CPPUNIT_TEST( testPort );
    CPPUNIT_TEST( testPort2 );
    CPPUNIT_TEST( testToString );
    CPPUNIT_TEST( testNavigateFile );
    CPPUNIT_TEST( testNavigateDots );
    CPPUNIT_TEST( testHttpFromFile );
    CPPUNIT_TEST( testSecurityFile );
    CPPUNIT_TEST( testSpaces );
    CPPUNIT_TEST_SUITE_END();

  public:
    void testSimpleHttp();
    void testBroken();
    void testNavigate();
    void testPort();
    void testToString();
    void testNavigateFile();
    void testNavigateDots();
    void testHttpFromFile();
    void testSecurityFile();
    void testPort2();
    void testSpaces();

};

#endif
