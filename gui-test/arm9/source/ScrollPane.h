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
#ifndef ScrollPane_h_seen
#define ScrollPane_h_seen

#include "Component.h"

class ScrollBar;

class ScrollPane: public Component
{

  public:
    ScrollPane();
    void setSize(unsigned int w, unsigned int h);
    void setLocation(unsigned int x, unsigned int y);
    void setScrollIncrement(int scrollIncrement);
    int scrollIncrement() const;

    void up();
    void down();
    void upBlock();
    void downBlock();

    /** Scroll to a "percentage" (0 = 0%, 256 = 100%) of the screen.
     * @param value the percentage of the screen to scroll to, represented in fixed point.
     */
    void scrollToPercent(int value);

    /** Set this to be the top level scroll pane. */
    void setTopLevel(bool topLevel=true);
    bool topLevel() const;

    void paint(const nds::Rectangle & clip);

    bool touch(int x, int y);

  private:
    int m_scrollIncrement;
    bool m_topLevel;
    bool m_canScrollUp;
    bool m_canScrollDown;
    ScrollBar * m_scrollBar;

    void layoutChildren();
    void calculateScrollBar();

    void showScrollBar(const nds::Rectangle & clip);
    void adjustScroll(int & scrollIncrement);
    void adjustScrollUp(int & scrollIncrement);
};

#endif
