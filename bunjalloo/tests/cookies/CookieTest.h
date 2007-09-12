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
    CPPUNIT_TEST_SUITE_END();

  public:
    void testBasic();
    void testRepeats();
    void testAccept();
    void setUp();
    void tearDown();

  private:
    CookieJar * m_cookieJar;

};

#endif
