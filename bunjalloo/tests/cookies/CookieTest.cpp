#include "CookieTest.h"
#include "CookieJar.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CookieTest );

void CookieTest::setUp()
{
  m_cookieJar = new CookieJar;
}
void CookieTest::tearDown()
{
  delete m_cookieJar;
}

void CookieTest::testBasic()
{
#if 0
  // this is from the server Set-Cookie (from HeaderParser):
  string requestHeader = "Set-Cookie: a=b\r\n";
  // this is what we return next time:
  string expectedHeader ="Cookie: a=b\r\n";
  m_cookieJar->addCookieHeader(requestHeader);
  string request;
  string resultHeader;
  m_cookieJar->cookiesForRequest(request, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);
#endif
}
