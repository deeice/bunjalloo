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
#include "Cache.h"
#include "CacheTest.h"
#include "Document.h"
#include "URI.h"
#include "File.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CacheTest );

void CacheTest::tearDown()
{
  delete m_cache;
  delete m_document;
  nds::File::rmrf("data");
}
void CacheTest::setUp()
{
  m_document = new Document;
  m_cache = new Cache(*m_document, true, true);
}

void CacheTest::test0()
{
  bool contains(m_cache->contains(URI("http://foo.com")));
  CPPUNIT_ASSERT(!contains);
}

void CacheTest::testLoadCheck()
{
  URI uri("http://www.google.com");
  bool contains(m_cache->contains(uri));
  CPPUNIT_ASSERT(!contains);
  bool result = m_cache->load(uri);
  CPPUNIT_ASSERT(!result);

  // now that it is in the cache, contains should be true
  contains = m_cache->contains(uri);
  CPPUNIT_ASSERT(contains);

  // now navigate to a new page
  URI next = uri.navigateTo("/index.html");
  result = m_cache->load(next);
  CPPUNIT_ASSERT(!result);

  // now that it is in the cache, contains should be true
  contains = m_cache->contains(next);
  CPPUNIT_ASSERT(contains);

  // now navigate to an internal link - should contain this,
  // as it is the same page.
  next = uri.navigateTo("/index.html#pos1");
  contains = m_cache->contains(next);
  CPPUNIT_ASSERT(contains);
}

