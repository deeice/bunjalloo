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
#include "SearchEntry.h"
#include "SearchTest.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( SearchTest );

void SearchTest::tearDown()
{
  delete m_search;
}
void SearchTest::setUp()
{
  m_search = new SearchEntry("input/search.cfg");
}

void SearchTest::test0()
{
  string result;
  bool check(m_search->checkSearch("g bunjalloo", result));
  CPPUNIT_ASSERT(check);
  string expected = "http://www.google.com/search?q=bunjalloo&btnG=Google+Search";
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void SearchTest::testInvalid()
{
  string result;
  bool valid(m_search->checkSearch("x bunjalloo", result));
  // should not be valid
  CPPUNIT_ASSERT(not valid);
}

void SearchTest::testPercentage()
{
  // tests for %s at the end of a line and with a %20 in the url
  string result;
  bool valid(m_search->checkSearch("s bunjalloo", result));
  CPPUNIT_ASSERT(valid);
  string expected = "http://checkpercentage/%20/bunjalloo";
  CPPUNIT_ASSERT_EQUAL(expected, result);
}
