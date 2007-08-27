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
#ifndef ScrollBar_h_seen
#define ScrollBar_h_seen

#include "Component.h"

class Scrollable;

class ScrollBar: public Component
{

  public:
    ScrollBar();

    void setTotal(int total);
    void setValue(int value);

    int total() const;
    int value() const;

    void setScrollable(Scrollable * scrollable);

    bool touch(int x, int y);

    void paint(const nds::Rectangle & clip);
    void setLocation(int x, int y);

  private:
    int m_total;
    int m_value;
    int m_handleSize;
    int m_handlePosition;
    Scrollable * m_scrollable;
    int m_handleHeld;

    void calculateHandle();

};
#endif
