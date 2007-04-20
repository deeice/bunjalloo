#include "UTF8.h"
#include "UTF8Test.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( UTF8Test );

#define TEST_DATA(d, r, eread) \
{\
  char * data = d;\
  const unsigned int expectedResult = r;\
  const unsigned int expectedRead = eread;\
  unsigned int actualResult;\
  unsigned int actualRead = UTF8::decode(data, actualResult);\
  CPPUNIT_ASSERT_EQUAL( expectedResult, actualResult ); \
  CPPUNIT_ASSERT_EQUAL( expectedRead, actualRead ); \
}\


// Lower boundary cases
void UTF8Test::testZero()
{
  TEST_DATA("\x00", 0x00, 1);
}

void UTF8Test::test80()
{
  TEST_DATA("\xc2\x80", 0x80, 2);
}
void UTF8Test::test800()
{
  TEST_DATA("\xe0\xa0\x80", 0x800, 3);
}
void UTF8Test::test1000()
{
  TEST_DATA("\xf0\x90\x80\x80", 0x10000, 4);
}
void UTF8Test::test20000()
{
  TEST_DATA("\xf8\x88\x80\x80\x80", 0x200000, 5);
}
void UTF8Test::test4000000()
{
  TEST_DATA("\xfc\x84\x80\x80\x80\x80", 0x4000000, 6);
}

// Upper boundary cases
void UTF8Test::test7f()
{
  TEST_DATA("\x7f", 0x7f, 1);
}
void UTF8Test::test7ff()
{
  TEST_DATA("\xdf\xbf", 0x7ff, 2);
}
void UTF8Test::testffff()
{
  TEST_DATA("\xEF\xBF\xBF",UTF8::MALFORMED, 3);
}
void UTF8Test::test1fffff()
{
  TEST_DATA("\xf7\xbf\xbf\xbf",0x1fffff, 4);
}
void UTF8Test::test3ffffff()
{
  TEST_DATA("\xfb\xbf\xbf\xbf\xbf",0x3ffffff, 5);
}
void UTF8Test::test7fffffff()
{
  TEST_DATA("\xfd\xbf\xbf\xbf\xbf\xbf",0x7fffffff, 6);
}

// mid boundary cases
void UTF8Test::testD7FF()
{
  TEST_DATA("\xED\x9f\xBF",0xd7ff, 3);
}
void UTF8Test::testE000()
{
  TEST_DATA("\xEE\x80\x80",0xE000, 3);
}
void UTF8Test::testFFFD()
{
  TEST_DATA("\xEF\xBF\xBD",0xFFFD, 3);
}
void UTF8Test::test10FFFF()
{
  TEST_DATA("\xf4\x8f\xbf\xbf",0x10FFFF, 4);
}
void UTF8Test::test110000()
{
  TEST_DATA("\xf4\x90\x80\x80",0x110000, 4);
}

// invalid start and end bytes
void UTF8Test::testFirstContinuation()
{
  TEST_DATA("\x80",UTF8::MALFORMED, 1);
}
void UTF8Test::testLastContinuation()
{
  TEST_DATA("\xbf",UTF8::MALFORMED, 1);
}
void UTF8Test::test2Continuations()
{
  // should only read 1 byte...
  TEST_DATA("\x80\xbf",UTF8::MALFORMED, 1);
}

void UTF8Test::testLonely2()
{
  // should only read 1 byte and return malformed.
  TEST_DATA("\xc0\x20",UTF8::MALFORMED, 1);
  TEST_DATA("\xdf\x20",UTF8::MALFORMED, 1);
}
void UTF8Test::testLonely3()
{
  // should only read 1 byte and return malformed.
  TEST_DATA("\xe0\x20",UTF8::MALFORMED, 1);
  TEST_DATA("\xef\x20",UTF8::MALFORMED, 1);
}
void UTF8Test::testLonely4()
{
  // should only read 1 byte and return malformed.
  TEST_DATA("\xf0\x20",UTF8::MALFORMED, 1);
  TEST_DATA("\xff\x20",UTF8::MALFORMED, 1);
}
void UTF8Test::testLonely5()
{
  // should only read 1 byte and return malformed.
  TEST_DATA("\xf8\x20",UTF8::MALFORMED, 1);
  TEST_DATA("\xfb\x20",UTF8::MALFORMED, 1);
}
void UTF8Test::testLonely6()
{
  // should only read 1 byte and return malformed.
  TEST_DATA("\xfc\x20",UTF8::MALFORMED, 1);
  TEST_DATA("\xfd\x20",UTF8::MALFORMED, 1);
}

void UTF8Test::testOverlong()
{
  TEST_DATA("\xc0\xaf",UTF8::MALFORMED, 2);
  TEST_DATA("\xe0\x80\xaf",UTF8::MALFORMED, 3);
  TEST_DATA("\xf0\x80\x80\xaf",UTF8::MALFORMED, 4);
  TEST_DATA("\xf8\x80\x80\x80\xaf",UTF8::MALFORMED, 5);
  TEST_DATA("\xfc\x80\x80\x80\x80\xaf",UTF8::MALFORMED, 6);
  // boundary cases..
  TEST_DATA("\xc1\xbf",UTF8::MALFORMED, 2);
  TEST_DATA("\xe0\x9f\xbf",UTF8::MALFORMED, 3);
  TEST_DATA("\xf0\x8f\xbf\xbf",UTF8::MALFORMED, 4);
  TEST_DATA("\xf8\x87\xbf\xbf\xbf",UTF8::MALFORMED, 5);
  TEST_DATA("\xfc\x83\xbf\xbf\xbf\xbf",UTF8::MALFORMED, 6);
  // NULL
  TEST_DATA("\xc0\x80",UTF8::MALFORMED, 2);
  TEST_DATA("\xe0\x80\x80",UTF8::MALFORMED, 3);
  TEST_DATA("\xf0\x80\x80\x80",UTF8::MALFORMED, 4);
  TEST_DATA("\xf8\x80\x80\x80\x80",UTF8::MALFORMED, 5);
  TEST_DATA("\xfc\x80\x80\x80\x80\x80",UTF8::MALFORMED, 6);
}

void UTF8Test::testInvalid()
{
   TEST_DATA("\xed\xa0\x80",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xad\xbf",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xae\x80",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xaf\xbf",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xb0\x80",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xbe\x80",UTF8::MALFORMED, 3);
   TEST_DATA("\xed\xbf\xbf",UTF8::MALFORMED, 3);
}

void UTF8Test::testCopyright()
{
  TEST_DATA("\xc2\xa9", 0xa9, 2);
}

void UTF8Test::testSymbolNE()
{
  TEST_DATA("\xe2\x89\xa0", 0x2260, 3);
}
