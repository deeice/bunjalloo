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
#include "Stylus.h"
#include "StylusTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( StylusTest );

void StylusTest::testClick()
{
  Stylus stylus;
  // unclick...
  stylus.update(false, 1, 1);

  // press once
  stylus.update(true, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::WAFFLE, stylus.clickType());

  // release
  stylus.update(false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::CLICK, stylus.clickType());
}

void StylusTest::testHold()
{
  Stylus stylus;
  // unclick...
  stylus.update(false, 1, 1);

  // press...
  stylus.update(true, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::WAFFLE, stylus.clickType());

  // hold..
  for (int i = 0; i < Stylus::HOLD_LIMIT; ++i)
  {
    stylus.update(true, 1, 1);
  }
  CPPUNIT_ASSERT_EQUAL(Stylus::HELD, stylus.clickType());

  // release
  stylus.update(false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::HELD, stylus.clickType());
}

void StylusTest::testHoldAndMove()
{
  Stylus stylus;
  // unclick...
  stylus.update(false, 1, 1);

  // press...
  stylus.update(true, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::WAFFLE, stylus.clickType());
  int x, y;
  stylus.startPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(1, x);
  CPPUNIT_ASSERT_EQUAL(1, y);

  // hold..
  for (int i = 0; i < Stylus::HOLD_LIMIT; ++i)
  {
    stylus.update(true, i, i);
  }
  stylus.update(true, 99, 27);
  stylus.startPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(1, x);
  CPPUNIT_ASSERT_EQUAL(1, y);

  stylus.endPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(99, x);
  CPPUNIT_ASSERT_EQUAL(27, y);
  CPPUNIT_ASSERT_EQUAL(Stylus::HELD, stylus.clickType());

  // release
  stylus.update(false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::HELD, stylus.clickType());
  stylus.endPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(99, x);
  CPPUNIT_ASSERT_EQUAL(27, y);
}
