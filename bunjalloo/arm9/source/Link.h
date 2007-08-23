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
#ifndef Link_h_seen
#define Link_h_seen
#include <string>
#include <list>
#include "Rectangle.h"

class Link
{
  public:
    Link(const std::string & a);
    ~Link();

    void appendClickZone(int x, int y, int w, int h);

    bool hitTest(int x, int y) const;
    std::string href() const;

    void highlight() const;

  private:
    std::string m_anchor;
    typedef std::list<nds::Rectangle*> RectangleList;
    RectangleList m_clickZones;
};

#endif
