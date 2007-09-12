/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "Rectangle.h"
#include <algorithm>

bool nds::Rectangle::hit(int testx, int testy) const
{
  return ( (testx >= x and testx < (x + w))
      and  (testy >= y and testy < (y + h)));
}
int nds::Rectangle::left() const
{
  return x; 
}
int nds::Rectangle::right() const
{
  return x+w; 
}
int nds::Rectangle::top() const
{
  return y;
}
int nds::Rectangle::bottom() const
{
  return y+h;
}

nds::Rectangle nds::Rectangle::intersect(const nds::Rectangle & r2) const
{
  const nds::Rectangle & r1(*this);
  bool haveIntersect =  not ( r1.right() < r2.left()
      or r1.left() > r2.right()
      or r1.top() > r2.bottom() 
      or r1.bottom() < r2.top()
      );
  Rectangle rect = {0, 0, 0, 0};
  if (not haveIntersect)
  {
    return rect;
  }
  using std::max;
  using std::min;
  rect.x = max(r1.left(), r2.left());
  rect.y = max(r1.top(), r2.top());
  int minRight = min(r1.right(), r2.right());
  rect.w = minRight-rect.x;
  int minBottom = min(r1.bottom(), r2.bottom());
  rect.h = minBottom-rect.y;

  return rect;
}

