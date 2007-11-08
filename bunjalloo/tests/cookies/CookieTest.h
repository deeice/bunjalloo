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
#ifndef CookieTest_h_seen
#define CookieTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class CookieJar;
class CookieTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( CookieTest );
    CPPUNIT_TEST( testBasic );
    CPPUNIT_TEST( testRepeats );
    CPPUNIT_TEST( testAccept );
    CPPUNIT_TEST( testSubDomain );
    CPPUNIT_TEST( testCalcTopLevel );
    CPPUNIT_TEST( testPath );
    CPPUNIT_TEST( testSecure );
    CPPUNIT_TEST( testGoogleLogin );
    CPPUNIT_TEST( testExpireRenew );
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();

    void testBasic();
    void testRepeats();
    void testAccept();
    void testSubDomain();
    void testCalcTopLevel();
    void testPath();
    void testSecure();
    void testGoogleLogin();
    void testExpireRenew();

  private:
    CookieJar * m_cookieJar;

};

#endif
