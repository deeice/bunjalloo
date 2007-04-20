#ifndef ParserTest_h_seen
#define ParserTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class HtmlParserDriver;
class HeaderParser;
class ParserTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ParserTest );
  CPPUNIT_TEST( test0 );
  CPPUNIT_TEST( testRedirect );
  CPPUNIT_TEST( testNotFound );
  CPPUNIT_TEST( testNotHtml );
  CPPUNIT_TEST( testIso );
  CPPUNIT_TEST( testBogusDoctype );
  CPPUNIT_TEST( testMetaIso );
  CPPUNIT_TEST( testSlashdot );
  CPPUNIT_TEST( testNumbers );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void test0();
  void testRedirect();
  void testNotFound();
  void testNotHtml();
  void testIso();
  void testBogusDoctype();
  void testMetaIso();
  void testSlashdot();
  void testNumbers();

  void readFile(const char * fileName);
private:
  HtmlParserDriver * m_htmlParser;
  HeaderParser * m_headerParser;
  char * m_data;
  unsigned int m_length;
};

#endif
