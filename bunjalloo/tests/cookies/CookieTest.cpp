#include "CookieTest.h"
#include "CookieJar.h"
#include "URI.h"

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
  URI uri("http://localhost/");
  // this is from the server Set-Cookie (from HeaderParser):
  string requestHeader = "a=b\r\n";
  // this is what we return next time:
  string expectedHeader ="Cookie: a=b\r\n";
  m_cookieJar->addCookieHeader(uri, requestHeader);
  string resultHeader;
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);
}

void CookieTest::testRepeats()
{
  URI uri("http://localhost/");
  // this is from the server Set-Cookie (from HeaderParser):
  string requestHeader = "a=b\r\n";
  // this is what we return next time:
  string expectedHeader ="Cookie: a=b\r\n";
  m_cookieJar->addCookieHeader(uri, requestHeader);
  m_cookieJar->addCookieHeader(uri, requestHeader);

  string resultHeader;
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);
}
