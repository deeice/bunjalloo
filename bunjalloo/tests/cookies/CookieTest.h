#ifndef CookieTest_h_seen
#define CookieTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class CookieJar;
class CookieTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( CookieTest );
    CPPUNIT_TEST( testBasic );
    CPPUNIT_TEST_SUITE_END();

  public:
    void testBasic();
    void setUp();
    void tearDown();

  private:
    CookieJar * m_cookieJar;

};

#endif
