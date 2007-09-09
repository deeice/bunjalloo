#include "HtmlElementTest.h"
#include "HtmlElement.h"
#include "HtmlTextAreaElement.h"
#include "HtmlMetaElement.h"
#include "HtmlOptionElement.h"
#include "HtmlInputElement.h"
#include "HtmlAnchorElement.h"
#include "ElementFactory.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( HtmlElementTest );

void HtmlElementTest::tearDown()
{
  delete m_element;
  delete m_clone;
}
void HtmlElementTest::setUp()
{
  m_element = 0;
  m_clone = 0;
}

void HtmlElementTest::test0()
{
  m_element = ElementFactory::create("html");
  CPPUNIT_ASSERT( m_element != 0);
}

void HtmlElementTest::testAttributes()
{
  m_element = ElementFactory::create("html");

  string expected("en");
  m_element->setAttribute("lang", string2unicode(expected));
  string result = unicode2string(m_element->attribute("lang"));
  CPPUNIT_ASSERT_EQUAL( expected, result);

  expected = "newId";
  m_element->setAttribute("id", string2unicode(expected));
  result = unicode2string(m_element->attribute("id"));
  CPPUNIT_ASSERT_EQUAL( expected, result);

  expected = "Test title";
  m_element->setAttribute("title", string2unicode(expected));
  result = unicode2string(m_element->attribute("title"));
  CPPUNIT_ASSERT_EQUAL( expected, result);

  expected = "fichero";
  m_element->setAttribute("dir", string2unicode(expected));
  result = unicode2string(m_element->attribute("dir"));
  CPPUNIT_ASSERT_EQUAL( expected, result);

  // try setting some unsupported attribute
  m_element->setAttribute("test", string2unicode(expected));
  result = unicode2string(m_element->attribute("test"));
  expected = "";
  CPPUNIT_ASSERT_EQUAL( expected, result);


}

void HtmlElementTest::testAppend()
{
  m_element = ElementFactory::create("html");
  HtmlElement * head = ElementFactory::create("head");
  m_element->append(head);
  CPPUNIT_ASSERT( m_element->hasChildren());
}

void HtmlElementTest::testIsa()
{
  m_element = ElementFactory::create("html");
  CPPUNIT_ASSERT( m_element->isa("html"));
  delete m_element;
  m_element = ElementFactory::create("head");
  CPPUNIT_ASSERT( m_element->isa("head"));
}


void HtmlElementTest::testAnchor()
{
  m_element = ElementFactory::create("a");
  CPPUNIT_ASSERT( m_element->isa("a"));
  HtmlAnchorElement * a = dynamic_cast<HtmlAnchorElement*>(m_element);
  CPPUNIT_ASSERT( a != 0 );
}


void HtmlElementTest::testClone()
{
  m_element = ElementFactory::create("a");
  m_element->setAttribute("href", string2unicode("http://localhost"));
  m_element->setAttribute("id", string2unicode("myElement"));
  m_clone = m_element->clone();
  CPPUNIT_ASSERT( m_clone != 0);
  CPPUNIT_ASSERT( m_clone->isa("a"));
  CPPUNIT_ASSERT_EQUAL(unicode2string(m_element->attribute("href")), 
      unicode2string(m_clone->attribute("href")));
  CPPUNIT_ASSERT_EQUAL(unicode2string(m_element->attribute("id")), 
      unicode2string(m_clone->attribute("id")));
}


void HtmlElementTest::testParent()
{
  // add a node, check its parent is the right one
  m_element = ElementFactory::create("html");
  HtmlElement * child = ElementFactory::create("head");
  m_element->append(child);

  CPPUNIT_ASSERT( child->parent() == m_element);
}

void HtmlElementTest::testAppendText()
{
  // parentNode -> add text, add node, add text 
  // check the text nodes are correct
  m_element = ElementFactory::create("body");
  m_element->appendText('T');
  CPPUNIT_ASSERT(m_element->hasChildren());
  HtmlElement * text = m_element->firstChild();
  string expected("#TEXT");
  CPPUNIT_ASSERT_EQUAL(expected, text->tagName());
  HtmlElement * a = ElementFactory::create("a");
  a->appendText('L');
  m_element->append(a);
  CPPUNIT_ASSERT_EQUAL(a, m_element->lastChild());
  CPPUNIT_ASSERT(a->hasChildren());
  m_element->appendText('2');
  CPPUNIT_ASSERT_EQUAL(expected, m_element->lastChild()->tagName());

}

void HtmlElementTest::testMeta()
{
  // create a meta node, then check the type
  m_element = ElementFactory::create("meta");
  CPPUNIT_ASSERT( m_element->isa("meta"));
  HtmlMetaElement * meta = dynamic_cast<HtmlMetaElement*>(m_element);
  CPPUNIT_ASSERT( meta != 0 );

  string expected("content-type");
  m_element->setAttribute("http-equiv", string2unicode("content-type"));
  CPPUNIT_ASSERT_EQUAL(expected, unicode2string(m_element->attribute("http-equiv")));
}

void HtmlElementTest::testRemove()
{
  // create a node, add another, check removal
  m_element = ElementFactory::create("body");
  m_element->appendText('T');
  HtmlElement * a = ElementFactory::create("a");
  HtmlElement * b = ElementFactory::create("b");
  m_element->append(a);
  m_element->append(b);
  size_t expected(3);
  CPPUNIT_ASSERT_EQUAL(expected, m_element->children().size());

  m_element->remove(a);
  expected = 2;
  CPPUNIT_ASSERT_EQUAL(expected, m_element->children().size());

  string expectedText("#TEXT");
  CPPUNIT_ASSERT_EQUAL(expectedText, m_element->firstChild()->tagName());
  expectedText = "b";
  CPPUNIT_ASSERT_EQUAL(expectedText, m_element->lastChild()->tagName());
  delete a;
}

void HtmlElementTest::testImg()
{
  // create a node, add another, check removal
  m_element = ElementFactory::create("img");
  m_element->setAttribute("src", string2unicode("image-src"));
  string expected("image-src");
  CPPUNIT_ASSERT_EQUAL(expected, unicode2string(m_element->attribute("src")));

  expected = "alternate text";
  m_element->setAttribute("alt", string2unicode(expected));
  CPPUNIT_ASSERT_EQUAL(expected, unicode2string(m_element->attribute("alt")));
  expected = "minombre";
  m_element->setAttribute("name", string2unicode(expected));
  CPPUNIT_ASSERT_EQUAL(expected, unicode2string( m_element->attribute("name")));
}

void HtmlElementTest::testPreviousSibling()
{
  m_element = ElementFactory::create("ul");
  HtmlElement * li = ElementFactory::create("li");
  HtmlElement * text = ElementFactory::create("#TEXT");
  m_element->append(text);
  m_element->append(li);
  const HtmlElement * ps = m_element->previousSibling(li);
  CPPUNIT_ASSERT(0 != ps);
  CPPUNIT_ASSERT(text == ps);
}

void HtmlElementTest::testNextSibling()
{
  m_element = ElementFactory::create("ul");
  HtmlElement * li = ElementFactory::create("li");
  HtmlElement * text = ElementFactory::create("#TEXT");
  m_element->append(text);
  m_element->append(li);
  const HtmlElement * ps = m_element->nextSibling(text);
  CPPUNIT_ASSERT(0 != ps);
  CPPUNIT_ASSERT(li == ps);
}

void HtmlElementTest::testOptionElement()
{
  m_element = ElementFactory::create("option");
  m_element->setAttribute("value",string2unicode("a"));
  m_element->setAttribute("label",string2unicode("b"));
  HtmlOptionElement * opt = dynamic_cast<HtmlOptionElement*>(m_element);
  CPPUNIT_ASSERT(opt != 0);
  CPPUNIT_ASSERT(not opt->selected());
  m_element->setAttribute("selected",string2unicode("yes"));
  CPPUNIT_ASSERT(opt->selected());
  
  string expected("a"); 
  CPPUNIT_ASSERT_EQUAL( expected , unicode2string(m_element->attribute("value")));
  expected = "b"; 
  CPPUNIT_ASSERT_EQUAL( expected , unicode2string(m_element->attribute("label")));
}


void HtmlElementTest::testElementsByTagName()
{
  m_element = ElementFactory::create("form");
  HtmlElement * select = ElementFactory::create("select");
  HtmlElement * option = ElementFactory::create("option");
  HtmlElement * option2 = ElementFactory::create("option");
  HtmlElement * p = ElementFactory::create("p");
  m_element->append(select);
  select->append(option);
  select->append(p);
  select->append(option2);

  size_t expected(1);
  CPPUNIT_ASSERT_EQUAL( expected , m_element->elementsByTagName("select").size());
  expected = 2;
  CPPUNIT_ASSERT_EQUAL( expected , select->elementsByTagName("option").size());


}

void HtmlElementTest::testTextAreaElement()
{
  m_element = ElementFactory::create("textarea");
  HtmlTextAreaElement * element = (HtmlTextAreaElement*)m_element;
  m_element->setAttribute("rows", string2unicode("10"));
  m_element->setAttribute("cols", string2unicode("80"));
  int expected = 10;
  int result = element->rows();
  CPPUNIT_ASSERT_EQUAL( expected, result);
  expected = 80;
  result = element->cols();
  CPPUNIT_ASSERT_EQUAL( expected, result);
}

void HtmlElementTest::testInputElement()
{
  m_element = ElementFactory::create("input");
  HtmlInputElement * element = (HtmlInputElement*)m_element;
  CPPUNIT_ASSERT(not element->hasAltText());

  UnicodeString altText = string2unicode("alt text");
  UnicodeString srcText = string2unicode("image.png");
  m_element->setAttribute("src", srcText);
  m_element->setAttribute("alt", altText);
  CPPUNIT_ASSERT_ASSERTION_PASS(altText == m_element->attribute("alt"));
  CPPUNIT_ASSERT_ASSERTION_PASS(srcText ==  m_element->attribute("src"));
}
