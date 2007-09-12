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
#ifndef TouchScreen_h_seen
#define TouchScreen_h_seen

#include "ScreenI.h"
#include <list>

class HotSpot;
//! Abstract class that implements touch screen.
class TouchScreen: public ScreenI
{
  public:
    ~TouchScreen();

    /** Get the last touch position as an X-Y tile position offset from the corner of the touch rectangle.
     * @param x Return value for the x tile position.
     * @param y Return value for the y tile position.
     */
    void lastTouchToTile16(int & x, int &y) const;

  protected:
    HotSpot * m_checking;
    int m_x;
    int m_y;
    std::list<HotSpot*> m_hotspots;
    //! Handle touch screen presses
    void handleTouch();
    void clearHotSpots();
};
#endif
