#include "ParamSetTest.h"
#include "ParameterSet.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParamSetTest );

void ParamSetTest::testBasic()
{
  string header = "a=b\r\n";
  ParameterSet paramSet(header);
  string expected = "b";
  string key = "a";
  string result;
  paramSet.parameter(key, result);
  CPPUNIT_ASSERT(paramSet.hasParameter(key));
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ParamSetTest::testQuote()
{
  string header = "key= 'my value'\n";
  ParameterSet paramSet(header);
  string expected = "my value";
  string key = "key";
  string result;
  paramSet.parameter(key, result);
  CPPUNIT_ASSERT(paramSet.hasParameter(key));
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ParamSetTest::testDoubleQuote()
{
  string header = "key = \"another	value\"\r\n";
  ParameterSet paramSet(header);
  string expected = "another	value";
  string key = "key";
  string result;
  paramSet.parameter(key, result);
  CPPUNIT_ASSERT(paramSet.hasParameter(key));
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

