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
#ifndef Rectangle_h_seen
#define Rectangle_h_seen

// tolua_begin
namespace nds
{
  struct Rectangle
  {
    int x;
    int y;
    int w;
    int h;
    bool hit(int x, int y) const;
    int right() const;
    int left() const;
    int top() const;
    int bottom() const;
    Rectangle intersect(const Rectangle & r2) const;
  };
}
// tolua_end
#endif
