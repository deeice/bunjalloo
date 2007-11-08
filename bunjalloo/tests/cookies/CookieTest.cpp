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
  m_cookieJar->setAcceptCookies("localhost");
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
  m_cookieJar->setAcceptCookies("localhost");
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

void CookieTest::testAccept()
{
  const string server("foobar.com");
  bool expected = false;
  bool result = m_cookieJar->acceptCookies(server);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Should reject foobar.com", expected, result);
  // now set to allow
  m_cookieJar->setAcceptCookies(server, true);
  expected = true;
  result = m_cookieJar->acceptCookies(server);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Should allow foobar.com", expected, result);
}

void CookieTest::testCalcTopLevel()
{
  const string alreadyTop("toplevel.com");
  string result = CookieJar::topLevel(alreadyTop);
  CPPUNIT_ASSERT_EQUAL(alreadyTop, result);

  const string subdomain("www.someurl.com");
  string expected = "someurl.com";
  result = CookieJar::topLevel(subdomain);
  CPPUNIT_ASSERT_EQUAL(expected, result);

  const string dotcom(".com");
  expected = ".com";
  result = CookieJar::topLevel(dotcom);
  CPPUNIT_ASSERT_EQUAL(expected, result);

  const string justcom("com");
  expected = "com";
  result = CookieJar::topLevel(justcom);
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void CookieTest::testSubDomain()
{
  const string settingServer("www.domain.com");
  const string subDomain("sub.domain.com");
  // accept cookies for all *.domain.com pages
  m_cookieJar->setAcceptCookies("domain.com");

  bool expected = true;
  bool result = m_cookieJar->acceptCookies(settingServer);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Should allow www.domain.com", expected, result);
  result = m_cookieJar->acceptCookies(subDomain);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Should allow sub.domain.com", expected, result);

  URI uri("http://sub.domain.com/");
  // now set cookie for sub domain - www.domain should not read it
  string requestHeader = "subcount=1\r\n";
  string expectedHeader = "Cookie: subcount=1\r\n";
  m_cookieJar->addCookieHeader(uri, requestHeader);

  string resultHeader;
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);
  // check www.domain.com doesn't return it
  uri.setUri("http://www.domain.com");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  expectedHeader = "";
  CPPUNIT_ASSERT(m_cookieJar->hasCookieForDomain(URI("sub.domain.com"),"subcount"));
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);


  // check that setting a cookie readable across all domains works
  // set for uri www.domain.com, but with domain=domain.com, ie. top level.
  requestHeader = "topcount=2;domain=domain.com;path=/\r\n";
  expectedHeader = "Cookie: topcount=2\r\n";
  m_cookieJar->addCookieHeader(uri, requestHeader);
  uri.setUri("http://some.domain.com");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT(m_cookieJar->hasCookieForDomain(URI("domain.com"),"topcount"));
  CPPUNIT_ASSERT(not m_cookieJar->hasCookieForDomain(URI("domain.com"),"subcount"));
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // test that for domain.com it includes the top level cookie
  uri.setUri("http://domain.com");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // now test that for sub.domain.com it returns the domain.com cookie AND the
  // sub.domain.com specific cookie.
  expectedHeader = "Cookie: subcount=1;topcount=2\r\n";
  uri.setUri("http://sub.domain.com");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

}

void CookieTest::testPath()
{
  const string settingServer("www.domain.com");
  const string subDomain("sub.domain.com");
  // accept cookies for all *.domain.com pages
  m_cookieJar->setAcceptCookies("domain.com");
  string requestHeader = "topcount=2;domain=domain.com;path=/accounts/\r\n";

  // add a path specific cookie
  URI uri("http://sub.domain.com/accounts/");
  m_cookieJar->addCookieHeader(uri, requestHeader);
  string expectedHeader = "Cookie: topcount=2\r\n";
  string resultHeader;
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  // check it works
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // check some other path cannot read it
  uri.setUri("http://sub.domain.com/");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  expectedHeader = "";
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // check a sub dir can read it
  uri.setUri("http://sub.domain.com/accounts/mine");
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  expectedHeader = "Cookie: topcount=2\r\n";
  CPPUNIT_ASSERT(m_cookieJar->hasCookieForDomain(uri,"topcount"));
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // test adding some unrelated site's cookie
  m_cookieJar->setAcceptCookies("elsewhere.com");
  requestHeader = "SD=richard;domain=elsewhere.com;path=/\r\n";
  uri.setUri("http://www.elsewhere.com/login");
  m_cookieJar->addCookieHeader(uri, requestHeader);

  expectedHeader = "Cookie: SD=richard\r\n";
  resultHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

}

void CookieTest::testGoogleLogin()
{
  // this test checks that setting a cookie from a cgi file correctly
  // changes the path to the directory name.

  m_cookieJar->setAcceptCookies("google.com");
  string requestHeader = "GALX=1234;Secure\r\n";
  URI uri("https://www.google.com/accounts/ServiceLogin?service=mail");
  m_cookieJar->addCookieHeader(uri, requestHeader);

  string expectedHeader = "Cookie: GALX=1234\r\n";
  string resultHeader;

  uri.setUri("https://www.google.com/accounts/ServiceLoginAuth?service=mail");
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

}

void CookieTest::testSecure()
{
  // don't send secure cookies via non secure connection
  m_cookieJar->setAcceptCookies("domain.com");

  URI uri("https://sub.domain.com/accounts/");
  string requestHeader = "LSID=ff9123;domain=domain.com;path=/accounts/;secure\r\n";
  m_cookieJar->addCookieHeader(uri, requestHeader);
  string expectedHeader = "Cookie: LSID=ff9123\r\n";
  string resultHeader;
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);

  // what about non secure?
  uri.setUri("http://sub.domain.com/accounts/");
  resultHeader = "";
  expectedHeader = "";
  m_cookieJar->cookiesForRequest(uri, resultHeader);
  CPPUNIT_ASSERT_EQUAL(expectedHeader, resultHeader);
}
