/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "UriTest.h"
#include "URI.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( UriTest );

void UriTest::testSimpleHttp()
{
  URI uri("http://test");
  string expected("test");
  CPPUNIT_ASSERT( uri.isValid());
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expected, uri.server());
  expected = "/";
  CPPUNIT_ASSERT_EQUAL( expected, uri.fileName());
}


void UriTest::testBroken()
{
  URI uri("htp:test", false);
  CPPUNIT_ASSERT( not uri.isValid());
}

void UriTest::testPort()
{
  URI uri("http://server/file");
  int expectedPort = 80;
  string expectedServer("server");
  string expectedFile("/file");
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.setUri("http://server:1500");
  expectedPort = 1500;
  expectedFile = "/";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.setUri("http://server:90/wiki/Documents:Main");
  expectedPort = 90;
  expectedFile = "/wiki/Documents:Main";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.setUri("http://server:");
  expectedPort = 80;
  expectedFile = "/";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.setUri("http://server/wiki/Documents:Main");
  expectedPort = 80;
  expectedFile = "/wiki/Documents:Main";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

}

void UriTest::testNavigate()
{
  URI uri("http://server/file");
  string expectedServer("server");
  string expectedFile("/file");
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("newFile");
  expectedFile = "/newFile";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("newPath/newfile.html");
  expectedFile = "/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("index.html");
  expectedFile = "/newPath/index.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("/index.html");
  expectedFile = "/index.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("/directory/");
  expectedFile = "/directory/";
  expectedServer = "server";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
}

void UriTest::testToString()
{
  URI uri("http://server/file");
  string expectedServer("server");
  string expectedFile("/file");
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  uri = uri.navigateTo("newPath/newfile.html");
  expectedFile = "/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  string expectedURI = "http://server/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedURI, uri.asString());

}

void UriTest::testNavigateFile()
{
  URI uri("file:///path/file");

  string expectedFile = "/path/file";
  CPPUNIT_ASSERT_EQUAL( URI::FILE_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("newPath/newfile.html");
  expectedFile = "/path/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("/root/index.txt");
  expectedFile = "/root/index.txt";
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

}

void UriTest::testNavigateDots()
{
  URI uri("http://server/file");
  string expectedServer("server");
  string expectedFile("/file");
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("../newfile.html");
  expectedFile = "/newfile.html";
  expectedServer = "server";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  string expectedURI = "http://server/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedURI, uri.asString());

  // make sure we can't go higher than root dir
  uri = uri.navigateTo("../newDir/index.html");
  expectedFile = "/newDir/index.html";
  expectedServer = "server";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  expectedURI = "http://server/newDir/index.html";
  CPPUNIT_ASSERT_EQUAL( expectedURI, uri.asString());

  // dots and absolute == rubbish
  uri = uri.navigateTo("/../another/file.html");
  expectedFile = "/another/file.html";
  expectedServer = "server";
  expectedURI = "http://server/another/file.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  CPPUNIT_ASSERT_EQUAL( expectedURI, uri.asString());

  // and dots later on..
  uri = uri.navigateTo("/another/directory/sub/../file.html");
  expectedFile = "/another/directory/file.html";
  expectedServer = "server";
  expectedURI = "http://server/another/directory/file.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  CPPUNIT_ASSERT_EQUAL( expectedURI, uri.asString());


}


void UriTest::testHttpFromFile()
{
  URI uri("file:///some/file.txt");

  string expectedFile = "/some/file.txt";
  CPPUNIT_ASSERT_EQUAL( URI::FILE_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri = uri.navigateTo("http://myserver.com/newfile.html");
  expectedFile = "/newfile.html";
  string expectedServer = "myserver.com";
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());

}

void UriTest::testSecurityFile()
{
  // should not be able to link to local URIs from http.
  URI uri("http://someserver.com/file.html");
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  uri = uri.navigateTo("file:///my/secret/file.txt");
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  uri = uri.navigateTo("config://options.html?proxy=123");
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
}

void UriTest::testPort2()
{
  URI uri("123.56.78.90:8080");
  int expectedPort = 8080;
  string expectedServer = "123.56.78.90";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri.port());

  // try breaking things
  URI uri2("123.56.78.90:");
  expectedPort = 80;
  expectedServer = "123.56.78.90";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri2.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri2.port());

  // try breaking things
  URI uri3("123.56.78.90/wherevr");
  expectedPort = 80;
  expectedServer = "123.56.78.90";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri3.server());
  CPPUNIT_ASSERT_EQUAL( expectedPort, uri3.port());
  URI breakage(uri3.navigateTo("/search?q=label:c"));
  string expectedFile("/search?q=label:c");
  CPPUNIT_ASSERT_EQUAL( expectedServer, breakage.server());

}

void UriTest::testSpaces()
{
  URI uri("http://server/some file with spaces.jpeg");
  string expectedFile = "/some%20file%20with%20spaces.jpeg";
  CPPUNIT_ASSERT_EQUAL( URI::HTTP_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
}

void UriTest::testHttps()
{
  URI uri("https://server/filename.html");
  string expectedFile = "/filename.html";
  CPPUNIT_ASSERT_EQUAL( URI::HTTPS_PROTOCOL, uri.protocol());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
}

void UriTest::testInternal()
{
  URI uri("http://server/filename.html");
  string expectedFile = "/filename.html";
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  const URI & internal(uri.navigateTo("#section1"));
  string expectedInternal("section1");
  CPPUNIT_ASSERT_EQUAL( expectedFile, internal.fileName());
  CPPUNIT_ASSERT_EQUAL( expectedInternal, internal.internalLink());
  const URI & internal2(internal.navigateTo("#section2"));
  expectedInternal = "section2";
  CPPUNIT_ASSERT_EQUAL( expectedInternal, internal2.internalLink());

}

void UriTest::testCRC()
{
  URI uri("http://server.com/");
  unsigned int crc = uri.crc32int();
  unsigned int expected(0x7fec9bc0);
  CPPUNIT_ASSERT_EQUAL(expected, crc);
}
