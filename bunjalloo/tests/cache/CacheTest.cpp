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

void CacheTest::readFile(const char * fileName)
{
  ifstream testFile;
  string inputFileName("input/");
  inputFileName += fileName;
  testFile.open(inputFileName.c_str(), ios::in);
  if (testFile.is_open())
  {
    testFile.seekg(0, ios::end);
    m_length = testFile.tellg();
    m_data = new char[m_length+2];
    testFile.seekg(0, ios::beg);
    testFile.read(m_data, m_length);
    m_data[m_length] = 0;
    testFile.close();
  }
  CPPUNIT_ASSERT(m_data != 0);
  CPPUNIT_ASSERT(m_length != 0);
}

void CacheTest::tearDown()
{
  delete m_cache;
  delete m_document;
  delete [] m_data;
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

  // magically load the data from a file and then
  // check that a new load returns "true", loaded from cache
  readFile("test.txt");
  m_document->appendData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  result = m_cache->load(next);
  CPPUNIT_ASSERT(result);

  // now navigate to an internal link
  next = uri.navigateTo("/index.html#pos1");
  // keep internal links, cache will not contain as it is a "new" link.
  contains = m_cache->contains(next, false);
  CPPUNIT_ASSERT(!contains);
  contains = m_cache->contains(next);
  CPPUNIT_ASSERT(contains);
  // should load from cache though, as it is the same base page.
  result = m_cache->load(next);
  CPPUNIT_ASSERT(result);

}
