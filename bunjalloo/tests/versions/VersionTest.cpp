/*
  Copyright (C) 2008 Richard Quirk

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
#include <gtest/gtest.h>
#include "Version.h"

TEST(VersionTest, 0)
{
  Version v1("0.5.5");
  Version v2("0.6");

  EXPECT_TRUE(v1 < v2);
  EXPECT_TRUE(v2 > v1);

}

TEST(VersionTest, 1)
{
  Version v1("1");
  Version v2("0");

  EXPECT_TRUE(v1 > v2);
  EXPECT_TRUE(v2 != v1);

}


TEST(VersionTest, 2)
{
  Version v1("1.0");
  Version v2("0.6.0");

  EXPECT_TRUE(v1 > v2);
  EXPECT_TRUE(v2 < v1);
  EXPECT_TRUE( !( v2 == v1) );

}

