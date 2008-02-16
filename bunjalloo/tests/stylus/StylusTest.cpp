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
#include "Stylus.h"
#include "StylusTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( StylusTest );

void StylusTest::testClick()
{
  Stylus stylus;
  // unclick...
  stylus.update(Stylus::NOTHING, false, 1, 1);

  // press once
  stylus.update(Stylus::DOWN, false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::DOWN, stylus.touchType());
  CPPUNIT_ASSERT(stylus.down());
  CPPUNIT_ASSERT(not stylus.isRepeat());

  // release
  stylus.update(Stylus::UP, false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::UP, stylus.touchType());
  CPPUNIT_ASSERT(stylus.up());
  CPPUNIT_ASSERT(not stylus.isRepeat());
}

void StylusTest::testHold()
{
  Stylus stylus;
  // unclick...
  stylus.update(Stylus::NOTHING, false, 1, 1);

  // press...
  stylus.update(Stylus::DOWN, false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::DOWN, stylus.touchType());

  stylus.update(Stylus::DOWN, true, 1, 1);
  // hold..
  CPPUNIT_ASSERT_EQUAL(Stylus::DOWN, stylus.touchType());
  CPPUNIT_ASSERT(stylus.down());
  CPPUNIT_ASSERT(stylus.isRepeat());

  // release
  stylus.update(Stylus::UP, true, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::UP, stylus.touchType());
  CPPUNIT_ASSERT(not stylus.isRepeat());
}

void StylusTest::testHoldAndMove()
{
  Stylus stylus;
  // unclick...
  stylus.update(Stylus::NOTHING, false, 1, 1);

  // press...
  stylus.update(Stylus::DOWN, false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::DOWN, stylus.touchType());
  int x, y;
  stylus.startPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(1, x);
  CPPUNIT_ASSERT_EQUAL(1, y);

  // hold..
  for (int i = 0; i < 200; ++i)
  {
    stylus.update(Stylus::DOWN, i&1, i, i);
  }
  stylus.update(Stylus::DOWN, true, 99, 27);
  stylus.startPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(1, x);
  CPPUNIT_ASSERT_EQUAL(1, y);

  stylus.lastPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(99, x);
  CPPUNIT_ASSERT_EQUAL(99, stylus.lastX());
  CPPUNIT_ASSERT_EQUAL(27, y);
  CPPUNIT_ASSERT_EQUAL(27, stylus.lastY());
  CPPUNIT_ASSERT_EQUAL(Stylus::DOWN, stylus.touchType());
  CPPUNIT_ASSERT(stylus.isRepeat());

  // release
  stylus.update(Stylus::UP, false, 1, 1);
  CPPUNIT_ASSERT_EQUAL(Stylus::UP, stylus.touchType());
  stylus.lastPoint(x, y);
  CPPUNIT_ASSERT_EQUAL(99, x);
  CPPUNIT_ASSERT_EQUAL(27, y);
}
