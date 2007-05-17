#ifndef ParamSetTest_h_seen
#define ParamSetTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class ParamSetTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( ParamSetTest );
    CPPUNIT_TEST( testBasic );
    CPPUNIT_TEST( testQuote );
    CPPUNIT_TEST( testDoubleQuote );
    CPPUNIT_TEST_SUITE_END();

  public:
    void testBasic();
    void testQuote();
    void testDoubleQuote();

};

#endif
