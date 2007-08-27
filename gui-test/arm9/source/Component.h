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
#ifndef Component_h_seen
#define Component_h_seen

#include <vector>
#include "Rectangle.h"

class Component
{
  public:
    Component();
    virtual ~Component();
    virtual void paint(const nds::Rectangle & clip);
    virtual void add(Component * child);

    /** Set location in coordinates of this Component's parent.
     */
    virtual void setLocation(unsigned int x, unsigned int y);

    virtual void setSize(unsigned int w, unsigned int h);
    nds::Rectangle bounds() const;
    nds::Rectangle preferredSize() const;
    inline int width() const;
    inline int height() const;
    inline int x() const;
    inline int y() const;

    virtual bool touch(int x, int y);

  protected:
    nds::Rectangle m_bounds;
    
    /** The width that this component would like to be. */
    int m_preferredWidth;
    /** The height that this component would like to be. */
    int m_preferredHeight;

    std::vector<Component *> m_children;
};

int Component::width() const
{
  return m_bounds.w;
}
int Component::height() const
{
  return m_bounds.h;
}

int Component::x() const
{
  return m_bounds.x;
}
int Component::y() const
{
  return m_bounds.y;
}

#endif
