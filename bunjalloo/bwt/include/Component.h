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

/** The base class for all GUI Widgets.*/
class Component
{
  public:
    /** Set up the Component.*/
    Component();
    
    /** End of the Component life - removes (deletes) all its child
     * Components.*/
    virtual ~Component();
    
    /** Paint the widget, clipping it to the given area.
     * @param clip the area to clip painting to.
     */
    virtual void paint(const nds::Rectangle & clip) = 0;

    /** Add a child Component.
     * @param child the child Component to add.
     */
    void add(Component * child);

    /** Remove and delete all child Components. */
    void removeChildren();

    /** Set the absolute on screen location.
     * @param x the horizontal x position.
     * @param y the vertical y position.
     */
    virtual void setLocation(unsigned int x, unsigned int y);

    /** Set the size in pixels.
     * @param w the width.
     * @param h the height.
     */
    virtual void setSize(unsigned int w, unsigned int h);
    /** Get the position and size.
     * @return a Rectangle that holds the horizontal and vertical coordinates
     * as well as the width and height of the Component.
     */
    nds::Rectangle bounds() const;

    /** Get the preferred size. This is the "ideal" size that the Component would like to be if it were allowed.
     * @return a Rectangle that holds the width and height of the preferred size. The x,y coordinates are meaningless.
     */
    nds::Rectangle preferredSize() const;

    /** Get the current width.
     * @return the width in pixels.
     */
    inline int width() const;

    /** Get the current height.
     * @return the height in pixels.
     */
    inline int height() const;

    /** Get the current horizontal x position.
     * @return the absolute x position.
     */
    inline int x() const;

    /** Get the current vertical y position.
     * @return the absolute y position.
     */
    inline int y() const;

    /** Handle user touch input.
     * @param x the on screen x position of the touch.
     * @param y the on screen y position of the touch.
     */
    virtual bool touch(int x, int y);

    inline bool visible() const;
    inline void setVisible(bool visible=true);

  protected:
    /** Holds the position and size. */
    nds::Rectangle m_bounds;
    
    /** The width that this component would like to be. */
    int m_preferredWidth;

    /** The height that this component would like to be. */
    int m_preferredHeight;

    /** The visibility. */
    bool m_visible;

    /** The child Components. */
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

bool Component::visible() const
{
  return m_visible;
}
void Component::setVisible(bool visible)
{
  m_visible = visible;
}

#endif
