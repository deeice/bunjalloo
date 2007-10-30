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
#ifndef SearchTest_h_seen
#define SearchTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class SearchEntry;
class SearchTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( SearchTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testInvalid );
  CPPUNIT_TEST( testPercentage );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test0();
  void testInvalid();
  void testPercentage();

private:
  SearchEntry * m_search;
};

#endif
