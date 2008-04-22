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
#include "ImageTest.h"
#include "Image.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ImageTest );

using nds::Image;

void ImageTest::setUp()
{
  m_image = 0;
}

void ImageTest::tearDown()
{
  delete m_image;
}

void ImageTest::testWide()
{
  m_image = new Image("inputs/wide.jpg");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 250;
  int result = m_image->width();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ImageTest::testTall()
{
  m_image = new Image("inputs/tall.jpg");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 382;
  int result = m_image->height();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ImageTest::testWidePng()
{
  m_image = new Image("inputs/wide.png");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 250;
  int result = m_image->width();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ImageTest::testTallPng()
{
  m_image = new Image("inputs/tall.png");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 382;
  int result = m_image->height();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ImageTest::testWideGif()
{
  m_image = new Image("inputs/wide.gif");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 250;
  int result = m_image->width();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ImageTest::testTallGif()
{
  m_image = new Image("inputs/tall.gif");
  CPPUNIT_ASSERT(m_image->isValid());
  int expected = 382;
  int result = m_image->height();
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

