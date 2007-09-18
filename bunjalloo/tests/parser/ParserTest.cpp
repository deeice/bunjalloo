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
#include <vector>
#include <fstream>
#include "HeaderParser.h"
#include "HtmlParserDriver.h"
#include "ParserTest.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );

void ParserTest::readFile(const char * fileName)
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

void ParserTest::tearDown()
{
  
  delete m_headerParser;
  delete m_htmlParser;
  if (m_data != 0)
  {
    delete [] m_data;
  }
}
void ParserTest::setUp()
{
  m_data = 0;
  m_length = 0;
  m_htmlParser = new HtmlParserDriver;
  m_headerParser = new HeaderParser(m_htmlParser, 0);
}

void ParserTest::test0()
{
  readFile("test0.txt");
  m_headerParser->feed(m_data, m_length);
  int size = m_htmlParser->m_tags.size();
  CPPUNIT_ASSERT_EQUAL( 2 , size);
}

void ParserTest::testRedirect()
{
  readFile("redirect.txt");
  m_headerParser->feed(m_data, m_length);
  string result = m_headerParser->redirect();
  string expected("http://code.google.com/");
  CPPUNIT_ASSERT_EQUAL( expected , result);
}


void ParserTest::testNotFound()
{
  readFile("404.txt");
  m_headerParser->feed(m_data, m_length);
  unsigned int result = m_headerParser->httpStatusCode();
  unsigned int expected = 404;
  CPPUNIT_ASSERT_EQUAL( expected , result);
}

void ParserTest::testNotHtml()
{
  readFile("not-html.txt");
  m_headerParser->feed(m_data, m_length);
  int result = m_htmlParser->m_tags.size();
  int expected = 0;
  CPPUNIT_ASSERT_EQUAL( expected , result);
  result =  m_htmlParser->m_data.size();
  expected = 48;
  CPPUNIT_ASSERT_EQUAL( expected , result);
}

void ParserTest::testIso()
{
  readFile("iso.txt");
  m_headerParser->feed(m_data, m_length);
  HtmlParser::Encoding result = m_htmlParser->encoding();
  HtmlParser::Encoding expected = HtmlParser::ISO_ENCODING;
  CPPUNIT_ASSERT_EQUAL( expected , result);
  int dataSize =  m_htmlParser->m_data.length();
  int expectedDataSize = 6;
  CPPUNIT_ASSERT_EQUAL( expectedDataSize , dataSize);
}

void ParserTest::testRefresh()
{
  readFile("refresh.txt");
  m_headerParser->feed(m_data, m_length);
  std::string refresh;
  int refreshTime;
  m_htmlParser->refresh(refresh, refreshTime);
  std::string expected("refresh2.html");
  CPPUNIT_ASSERT_EQUAL( expected , refresh);
  int expectedTime = 6;
  CPPUNIT_ASSERT_EQUAL( expectedTime , refreshTime);
}

void ParserTest::testBogusDoctype()
{
  readFile("bogus-doctype.txt");
  m_headerParser->feed(m_data, m_length);
  int result = m_htmlParser->m_data.size();
  CPPUNIT_ASSERT_EQUAL( 0 , result);
}

void ParserTest::testMetaIso()
{
  readFile("metaiso.txt");
  m_headerParser->feed(m_data, m_length);
  HtmlParser::Encoding result = m_htmlParser->encoding();
  HtmlParser::Encoding expected = HtmlParser::ISO_ENCODING;
  CPPUNIT_ASSERT_EQUAL( expected , result);
}
void ParserTest::testSlashdot()
{
  readFile("slashdot.txt");
  m_headerParser->feed(m_data, m_length);
  int size = m_htmlParser->m_tags.size();
  CPPUNIT_ASSERT_EQUAL( 2 , size);
}

void ParserTest::testNumbers()
{
  readFile("numbers.txt");
  m_headerParser->feed(m_data, m_length);
  int size = m_htmlParser->m_tags.size();
  CPPUNIT_ASSERT_EQUAL( 1 , size);
}
