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

#include "HotSpot.h"

HotSpot::HotSpot(nds::Rectangle & rect, 
    HotSpotCallback_t callback, 
    void * data)
: m_area(rect),
  m_callback(callback),
  m_data(data)
{
}
void HotSpot::hitTest(int x, int y)
{
  if (m_area.hit(x,y) and m_callback)
  {
    m_callback(m_data);
  }
}
