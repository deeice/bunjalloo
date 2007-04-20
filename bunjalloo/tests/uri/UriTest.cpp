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
  CPPUNIT_ASSERT( not uri.isFile());
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

  uri.navigateTo("newFile");
  expectedFile = "/newFile";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.navigateTo("newPath/newfile.html");
  expectedFile = "/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.navigateTo("index.html");
  expectedFile = "/newPath/index.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.navigateTo("/index.html");
  expectedFile = "/index.html";
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
}

void UriTest::testToString()
{
  URI uri("http://server/file");
  string expectedServer("server");
  string expectedFile("/file");
  CPPUNIT_ASSERT_EQUAL( expectedServer, uri.server());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());
  uri.navigateTo("newPath/newfile.html");
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
  CPPUNIT_ASSERT( uri.isFile());
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.navigateTo("newPath/newfile.html");
  expectedFile = "/path/newPath/newfile.html";
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

  uri.navigateTo("/root/index.txt");
  expectedFile = "/root/index.txt";
  CPPUNIT_ASSERT_EQUAL( expectedFile, uri.fileName());

}
