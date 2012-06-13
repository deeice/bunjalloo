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
#include <algorithm>
#include "libnds.h"
#include "HotSpot.h"
#include "TouchScreen.h"

static void rmHS(HotSpot * hs)
{
  delete hs;
}
TouchScreen::~TouchScreen() 
{
  clearHotSpots();
}
void TouchScreen::clearHotSpots()
{
  std::for_each(m_hotspots.begin(), m_hotspots.end(), rmHS);
  m_hotspots.clear();
}

void TouchScreen::handleTouch()
{
  touchPosition tp;
  touchRead(&tp);
  m_x = tp.px;
  m_y = tp.py;
  std::list<HotSpot*>::iterator it(m_hotspots.begin());
  for (; it != m_hotspots.end(); ++it)
  {
    HotSpot * spot(*it);
    m_checking = spot;
    spot->hitTest(m_x, m_y);
  }
}

void TouchScreen::lastTouchToTile16(int & x, int &y) const
{
  x = (m_x - m_checking->area().x)/16;
  y = (m_y - m_checking->area().y)/16;
}
