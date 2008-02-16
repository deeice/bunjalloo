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

  string refresh="0; url='redirected.html'";
  ParameterSet paramSet2(refresh);
  expected = "redirected.html";
  key = "url";
  result = "";
  CPPUNIT_ASSERT(paramSet2.hasParameter(key));
  paramSet2.parameter(key, result);
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

void ParamSetTest::testAmphersand()
{
  string header = "proxy=234.56.78.90&proxyOn=no";
  ParameterSet paramSet(header, '&');
  string expected = "234.56.78.90";
  string key = "proxy";
  string result;
  paramSet.parameter(key, result);
  CPPUNIT_ASSERT(paramSet.hasParameter(key));
  CPPUNIT_ASSERT_EQUAL(expected, result);
  key = "proxyOn";
  expected = "no";
  paramSet.parameter(key, result);
  CPPUNIT_ASSERT(paramSet.hasParameter(key));
  CPPUNIT_ASSERT_EQUAL(expected, result);


}

