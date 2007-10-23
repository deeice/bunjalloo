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
#ifndef ImageTest_h_seen
#define ImageTest_h_seen

#include <cppunit/extensions/HelperMacros.h>

class Image;
class ImageTest : public CPPUNIT_NS::TestFixture
{
  private:
    CPPUNIT_TEST_SUITE( ImageTest );
    CPPUNIT_TEST( testWide );
    CPPUNIT_TEST( testTall );
    CPPUNIT_TEST( testWidePng );
    CPPUNIT_TEST( testTallPng );
    CPPUNIT_TEST( testWideGif );
    CPPUNIT_TEST( testTallGif );
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();
    void testWide();
    void testTall();
    void testWidePng();
    void testTallPng();
    void testWideGif();
    void testTallGif();
  private:
    Image * m_image;

};

#endif
