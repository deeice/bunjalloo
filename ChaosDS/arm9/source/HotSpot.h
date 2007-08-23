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
#ifndef HotSpot_h_seen
#define HotSpot_h_seen

#include "Rectangle.h"

typedef void (*HotSpotCallback_t)(void*);

class HotSpot
{
  public:
    HotSpot(nds::Rectangle & rect, HotSpotCallback_t callback, void * data=0);
    void hitTest(int x, int y);
    inline const nds::Rectangle & area() const
    {
      return m_area;
    }


  private:
    nds::Rectangle m_area;
    HotSpotCallback_t m_callback;
    void * m_data;
};
#endif
