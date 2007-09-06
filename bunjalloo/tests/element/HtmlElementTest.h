#ifndef HtmlElementTest_h_seen
#define HtmlElementTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class HtmlElement;
class HtmlElementTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( HtmlElementTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testAttributes );
  CPPUNIT_TEST( testAppend );
  CPPUNIT_TEST( testIsa );
  CPPUNIT_TEST( testAnchor );
  CPPUNIT_TEST( testClone );
  CPPUNIT_TEST( testParent );
  CPPUNIT_TEST( testAppendText );
  CPPUNIT_TEST( testMeta );
  CPPUNIT_TEST( testRemove );
  CPPUNIT_TEST( testImg );
  CPPUNIT_TEST( testPreviousSibling );
  CPPUNIT_TEST( testNextSibling );
  CPPUNIT_TEST( testOptionElement );
  CPPUNIT_TEST( testElementsByTagName );
  CPPUNIT_TEST( testTextAreaElement );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test0();
  void testAttributes();
  void testAppend();
  void testIsa();
  void testAnchor();
  void testClone();

  void testParent();
  void testAppendText();
  void testMeta();
  void testRemove();
  void testImg();
  void testPreviousSibling();
  void testNextSibling();
  void testOptionElement();
  void testElementsByTagName();
  void testTextAreaElement();

private:
  HtmlElement * m_element;
  HtmlElement * m_clone;
};

#endif
