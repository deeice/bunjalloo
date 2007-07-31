#ifndef UriTest_h_seen
#define UriTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class UriTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( UriTest );
    CPPUNIT_TEST( testSimpleHttp );
    CPPUNIT_TEST( testBroken );
    CPPUNIT_TEST( testNavigate );
    CPPUNIT_TEST( testPort );
    CPPUNIT_TEST( testPort2 );
    CPPUNIT_TEST( testToString );
    CPPUNIT_TEST( testNavigateFile );
    CPPUNIT_TEST( testNavigateDots );
    CPPUNIT_TEST( testHttpFromFile );
    CPPUNIT_TEST( testSecurityFile );
    CPPUNIT_TEST_SUITE_END();

  public:
    void testSimpleHttp();
    void testBroken();
    void testNavigate();
    void testPort();
    void testToString();
    void testNavigateFile();
    void testNavigateDots();
    void testHttpFromFile();
    void testSecurityFile();
    void testPort2();

};

#endif
