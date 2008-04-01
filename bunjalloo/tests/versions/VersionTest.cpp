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
#include "Version.h"

using namespace std;

class VersionTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( VersionTest );
    CPPUNIT_TEST( test0 );
    CPPUNIT_TEST( test1 );
    CPPUNIT_TEST( test2 );
    CPPUNIT_TEST_SUITE_END();

  public:
    void test0();
    void test1();
    void test2();
  private:

};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VersionTest );

void VersionTest::test0()
{
  Version v1("0.5.5");
  Version v2("0.6");

  CPPUNIT_ASSERT(v1 < v2);
  CPPUNIT_ASSERT(v2 > v1);

}

void VersionTest::test1()
{
  Version v1("1");
  Version v2("0");

  CPPUNIT_ASSERT(v1 > v2);
  CPPUNIT_ASSERT(v2 != v1);

}


void VersionTest::test2()
{
  Version v1("1.0");
  Version v2("0.6.0");

  CPPUNIT_ASSERT(v1 > v2);
  CPPUNIT_ASSERT(v2 < v1);
  CPPUNIT_ASSERT( !( v2 == v1) );

}

