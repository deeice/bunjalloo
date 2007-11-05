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
#include <iostream>
#include <list>
#include <fstream>
#include "HtmlParser.h"
#include "DocumentTest.h"
#include "HtmlElement.h"
#include "HtmlDocument.h"
#include "HeaderParser.h"
#include "Document.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DocumentTest );

void DocumentTest::readFile(const char * fileName)
{
  ifstream testFile;
  string inputFileName("input/");
  inputFileName += fileName;
  testFile.open(inputFileName.c_str(), ios::in);
  CPPUNIT_ASSERT(testFile.is_open());
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

void DocumentTest::tearDown()
{
  
  delete m_document;
  if (m_data != 0)
  {
    delete [] m_data;
  }
}

void DocumentTest::setUp()
{
  m_data = 0;
  m_length = 0;
  m_document = new Document;
}

void DocumentTest::test0()
{
  const string expected("file:///test0.txt");
  m_document->setUri(expected);
  string result = m_document->uri();
  CPPUNIT_ASSERT_EQUAL( expected , result);
}

void DocumentTest::test1()
{
  readFile("test1.txt");
  m_document->appendData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * result = m_document->rootNode();
  CPPUNIT_ASSERT( result != 0);
}

void DocumentTest::testHtmlAttributes()
{
  readFile("html-attrs.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * result = m_document->rootNode();
  string expected("en");
  string attribResult = unicode2string(result->attribute("lang"));
  CPPUNIT_ASSERT_EQUAL(expected, attribResult);
}

void DocumentTest::testHead()
{
  readFile("head.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * result = m_document->rootNode();
  CPPUNIT_ASSERT(result->hasChildren());
  const HtmlElement * child = result->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));
}

void DocumentTest::testEmpty()
{
  readFile("empty.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * result = m_document->rootNode();
  CPPUNIT_ASSERT(result != 0);
}

void DocumentTest::testHead2()
{
  readFile("head2.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * result = m_document->rootNode();
  CPPUNIT_ASSERT(result->hasChildren());
  const HtmlElement * child = result->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));
}

void DocumentTest::testHead3()
{
  readFile("head3.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * result = m_document->rootNode();
  CPPUNIT_ASSERT(result->hasChildren());
  const HtmlElement * child = result->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));
  const HtmlElement * meta = child->firstChild();
  CPPUNIT_ASSERT(meta != 0);
  CPPUNIT_ASSERT(meta->isa("meta"));

  ElementList::const_iterator it(result->children().begin());
  int index(0);
  for (; it != result->children().end(); ++it,++index)
  {
    HtmlElement * element(*it);
    if (index == 0) {
    CPPUNIT_ASSERT(element->isa("head"));
    }
    else if (index == 1)
    {
    CPPUNIT_ASSERT(element->isa("body"));
    }
  }
}

void DocumentTest::testTitle()
{
  readFile("title.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root->hasChildren());
  const HtmlElement * child = root->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));
  const HtmlElement * meta = child->firstChild();
  CPPUNIT_ASSERT(meta != 0);
  CPPUNIT_ASSERT(meta->isa("title"));

  const ElementList & rootChilds = root->children();
  CPPUNIT_ASSERT(rootChilds.size() > 1);
  ElementList::const_iterator it(rootChilds.begin());
  int index(0);
  for (; it != rootChilds.end(); ++it,++index)
  {
    HtmlElement * element(*it);
    if (index == 1) {
      CPPUNIT_ASSERT(element->isa("body"));
    }
  }

}

void DocumentTest::testGoogle()
{
  readFile("google.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
}


void DocumentTest::testAnchor()
{
  readFile("anchor.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);

  const HtmlElement * child = root->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));

  const ElementList & rootChilds = root->children();
  ElementList::const_iterator it(rootChilds.begin());
  int index(0);
  for (; it != rootChilds.end(); ++it,++index)
  {
    HtmlElement * element(*it);
    if (index == 1) {
      CPPUNIT_ASSERT(element->isa("body"));
      CPPUNIT_ASSERT(element->hasChildren());
      const HtmlElement * a = element->firstChild();
      CPPUNIT_ASSERT(a != 0);
      CPPUNIT_ASSERT(a->isa("a"));
      string href = unicode2string(a->attribute("href"));
      string expected("anchor");
      CPPUNIT_ASSERT_EQUAL(expected, href);
    }
  }
}


void DocumentTest::testBrokenAnchor()
{
  readFile("anchor-broken.html");
  m_document->appendLocalData(m_data, m_length);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);

  const HtmlElement * child = root->firstChild();
  CPPUNIT_ASSERT(child != 0);
  CPPUNIT_ASSERT(child->isa("head"));

  const ElementList & rootChilds = root->children();
  ElementList::const_iterator it(rootChilds.begin());
  int index(0);
  for (; it != rootChilds.end(); ++it,++index)
  {
    HtmlElement * element(*it);
    if (index == 1) {
      CPPUNIT_ASSERT(element->isa("body"));
      CPPUNIT_ASSERT(element->hasChildren());
      const HtmlElement * a = element->firstChild();
      CPPUNIT_ASSERT(a != 0);
      CPPUNIT_ASSERT(a->isa("a"));
      string href = unicode2string(a->attribute("href"));
      string expected("anchor");
      CPPUNIT_ASSERT_EQUAL(expected, href);
    }
  }
}

void DocumentTest::testCharacterStart()
{
  readFile("character-start.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
}
void DocumentTest::testEndTagStart()
{
  readFile("endtag-start.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  string rootType = root->tagName();
  string expected("html");
  CPPUNIT_ASSERT_EQUAL(expected, rootType);
}

void DocumentTest::testSimpleBodyA()
{
  readFile("simple.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  string expected("html");
  CPPUNIT_ASSERT_EQUAL( expected, root->tagName());

  CPPUNIT_ASSERT(root->hasChildren());

  const ElementList & children = root->children();
  ElementList::const_iterator childIt(children.begin());
  expected = "head";
  CPPUNIT_ASSERT_EQUAL(expected, (*childIt)->tagName());
  // now check the head:
  {
    // meta and title
    HtmlElement * head = *childIt;
    ElementList::const_iterator headIt(head->children().begin());
    expected = "meta";
    CPPUNIT_ASSERT_EQUAL(expected, (*headIt)->tagName());
    expected = "content-type";
    CPPUNIT_ASSERT_EQUAL(expected, unicode2string((*headIt)->attribute("http-equiv")));
    ++headIt;
    expected = "title";
    CPPUNIT_ASSERT_EQUAL(expected, (*headIt)->tagName());
    HtmlElement * title = *headIt;
    CPPUNIT_ASSERT(title->hasChildren());
    expected = "#TEXT";
    CPPUNIT_ASSERT_EQUAL(expected, title->firstChild()->tagName());
    ++headIt;
  }
  ++childIt;
  expected = "body";
  CPPUNIT_ASSERT_EQUAL(expected, (*childIt)->tagName());
  ++childIt;
  CPPUNIT_ASSERT(children.end() == childIt);
}

void DocumentTest::testMismatchFormat()
{
  readFile("mismatch-format.html");
  // should produce this:
  // html >
  //   head >
  //     body >
  //        #text
  //        b > 
  //           #text (bold)
  //           i > 
  //              #text (bolditalic)
  //        i >
  //           #text (italic)
  //           a > 
  //               #text (link?italic?) 
  //        #text  ()
  //        a >
  //            #text ()
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  string rootType = root->tagName();
  string expected("html");
  CPPUNIT_ASSERT_EQUAL(expected, rootType);

}

void DocumentTest::testLi()
{
  readFile("test-li.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  HtmlElement * body = root->lastChild();
  CPPUNIT_ASSERT(body != 0);
  CPPUNIT_ASSERT(body->isa("body"));
  CPPUNIT_ASSERT(body->firstChild()->isa("li"));
}

void DocumentTest::testDD()
{
  readFile("test-dd.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  HtmlElement * body = root->lastChild();
  CPPUNIT_ASSERT(body != 0);
  CPPUNIT_ASSERT(body->isa("body"));
  CPPUNIT_ASSERT(body->firstChild()->isa("dd"));
}

void DocumentTest::testPlaintext()
{
  readFile("plaintext.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}


void DocumentTest::testFont()
{
  readFile("font.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}

void DocumentTest::testFont2()
{
  readFile("font2.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}

void DocumentTest::testPin8()
{
  readFile("pineight.txt");
  // tests for end script tag after end of chunk
  m_document->appendData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}


void DocumentTest::testClarin()
{
  readFile("clarin.txt");
  // tests for end script tag after end of chunk
  // send bytes in chunks, emulates http request.
  int length = 1360;
  char * data = m_data;
  m_document->appendData(data, length);
  data += length;
  m_document->appendData(data, length);
  data += length;
  m_document->appendData(data, length);
  data += length;
  length = 4080;
  m_document->appendData(data, length);
  data += length;
  length = 1360;
  m_document->appendData(data, length);
  data += length;
  length = 2720;
  m_document->appendData(data, 2720);
  data += length;
  length = 6800;
  m_document->appendData(data, 6800);
  data += length;
  length = 1360;
  m_document->appendData(data, 1360);
  data += length;
  m_document->appendData(data, 1360);
  data += length;
  length = 2252;
  m_document->appendData(data, 2252);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  CPPUNIT_ASSERT(root->hasChildren());
}


void DocumentTest::testAdoption()
{
  readFile("adoption.html");
  // test the adoption algorithm
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  const HtmlElement * body = root->lastChild();
  const ElementList & children = body->children();
  int childcount = children.size();
  CPPUNIT_ASSERT_EQUAL(2, childcount);
}

void DocumentTest::testAdoption2()
{
  readFile("adoption2.html");
  // test the adoption algorithm
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}

void DocumentTest::testHeader()
{
  readFile("header1.html");
  // test the adoption algorithm
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  const HtmlElement * body = root->lastChild();
  CPPUNIT_ASSERT(body != 0);
  CPPUNIT_ASSERT(body->isa("body"));
  const HtmlElement * h1(0);
  ElementList::const_iterator it(body->children().begin());
  for (; it != body->children().end(); ++it)
  {
    const HtmlElement * element(*it);
    if ( element->isa("h1"))
    {
      h1 = element;
      break;
    }
  }
  CPPUNIT_ASSERT(it != body->children().end());
  CPPUNIT_ASSERT(h1 != 0);
}

void DocumentTest::testHeader2()
{
  readFile("header2.html");
  // test the adoption algorithm
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  const HtmlElement * body = root->lastChild();
  CPPUNIT_ASSERT(body != 0);
  CPPUNIT_ASSERT(body->isa("body"));
  const HtmlElement * h1(0);
  const HtmlElement * h2(0);
  ElementList::const_iterator it(body->children().begin());
  for (; it != body->children().end(); ++it)
  {
    const HtmlElement * element(*it);
    if ( element->isa("h1"))
    {
      h1 = element;
    }
    if ( element->isa("h2"))
    {
      h2 = element;
    }
  }
  CPPUNIT_ASSERT(h1 != 0);
  CPPUNIT_ASSERT(h2 != 0);
}

void DocumentTest::testAttribs()
{
  readFile("attrib.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
}

void DocumentTest::testUnicode2String()
{
  
  UnicodeString uc;
  uc += 0xa9;
  string c = unicode2string(uc);
  string expected = "%C2%A9";
  CPPUNIT_ASSERT_EQUAL(expected, c);
}

void DocumentTest::testActiveFormatters()
{
  readFile("issue29.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));

  // get the body node
  ElementList bodyNodes = root->elementsByTagName("body");
  CPPUNIT_ASSERT(bodyNodes.size() == 1);

  const HtmlElement * body = bodyNodes.front();
  const HtmlElement * bold = body->firstChild();
  CPPUNIT_ASSERT(bold->isa("b"));

  ElementList pNodes = bold->elementsByTagName("p");
  CPPUNIT_ASSERT(pNodes.size() == 2);

  // now see if the 2nd p node has multiple children
  const HtmlElement * p = pNodes.back();
  CPPUNIT_ASSERT(p->children().size() == 1);


  const HtmlElement * child = p->firstChild();
  // see how many recursive children the p has
  int depth = 0;
  while (child->hasChildren())
  {
    child = child->firstChild();
    depth++;
  }
  int expectedDepth = 1;
  CPPUNIT_ASSERT_EQUAL(expectedDepth, depth);
}

void DocumentTest::testHistory()
{
  readFile("attrib.html");
  m_document->setUri("attrib.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setPosition(10);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));

  delete [] m_data;
  readFile("anchor.html");
  m_document->setUri("anchor.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  CPPUNIT_ASSERT_EQUAL(0, m_document->position());

  string prev = m_document->gotoPreviousHistory();

  string expected = "attrib.html";
  CPPUNIT_ASSERT_EQUAL(expected, prev);
  CPPUNIT_ASSERT_EQUAL(10, m_document->position());
}

void DocumentTest::testBodyEnd()
{
  readFile("body.html");
  m_document->appendLocalData(m_data, m_length);
  m_document->setStatus(Document::LOADED);
  const HtmlElement * root = m_document->rootNode();
  CPPUNIT_ASSERT(root != 0);
  CPPUNIT_ASSERT(root->isa("html"));
  const HtmlElement * body = root->lastChild();
  CPPUNIT_ASSERT(body != 0);
  CPPUNIT_ASSERT(body->isa("body"));
}
