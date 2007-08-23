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
#include "Canvas.h"
using namespace nds;

void Canvas::setClip(const Rectangle & clip)
{
  m_clip = clip;
  if (m_clip.x < 0)
  {
    m_clip.x = 0;
  }
  if (m_clip.y < 0)
  {
    m_clip.y = 0;
  }
  if (m_clip.w > width())
  {
    m_clip.w = width();
  }
  if (m_clip.h > height())
  {
    m_clip.h = height();
  }

}

Rectangle Canvas::clip() const
{
  return m_clip;
}

void Canvas::verticalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x, y+i, colour);
  }
}

void Canvas::horizontalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x+i, y, colour);
  }
}

void Canvas::drawRectangle(int x, int y, int w, int h, int colour)
{
  horizontalLine(x,y,w,colour);
  horizontalLine(x,y+h,w,colour);
  verticalLine(x,y,h,colour);
  verticalLine(x+w,y,h,colour);
}

void Canvas::fillRectangle(int x, int y, int w, int h, int colour)
{
  for (int j = 0; j < h; j++) {
    horizontalLine(x, y+j, w, colour);
  }
}


