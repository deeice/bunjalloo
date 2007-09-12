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
