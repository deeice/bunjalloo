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
#ifndef ParamSetTest_h_seen
#define ParamSetTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class ParamSetTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( ParamSetTest );
    CPPUNIT_TEST( testBasic );
    CPPUNIT_TEST( testQuote );
    CPPUNIT_TEST( testDoubleQuote );
    CPPUNIT_TEST( testAmphersand );
    CPPUNIT_TEST_SUITE_END();

  public:
    void testBasic();
    void testQuote();
    void testDoubleQuote();
    void testAmphersand();

};

#endif
