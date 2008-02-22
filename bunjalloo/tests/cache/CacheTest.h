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
#ifndef CacheTest_h_seen
#define CacheTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class Cache;
class Document;
class CacheTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( CacheTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testLoadCheck );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test0();
  void testLoadCheck();

private:
  Cache * m_cache;
  Document * m_document;
  char * m_data;
  int m_length;
  void readFile(const char * fileName);
};

#endif
