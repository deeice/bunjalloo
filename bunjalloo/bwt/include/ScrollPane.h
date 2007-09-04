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

/** A scrollable view of many Components. The view may be larger than the
 * screen and this class handles the positioning of all the child widgets. */
class ScrollPane: public Component
{

  public:
    /** Set a pop up menu that will override the display and input handling.
     * @param popup the menu item to show.
     */
    static void setPopup(Component * popup);

    /** Remove a pop up menu.
     * @param popup the menu item to show, if the current menu is not this one,
     * then nothing is done.
     */
    static void removePopup(Component * popup);
    
    /** Initialise the ScrollPane.*/
    ScrollPane();
    
    /** Delete the ScrollPanes child components.*/
    ~ScrollPane();

    /** Set the incremental amount that each up/down press moves.
     * @param scrollIncrement the amount to scroll in pixels.
     */
    void setScrollIncrement(int scrollIncrement);

    /** Get the current scroll increment.
     * @return the scroll increment in pixels.
     */
    int scrollIncrement() const;

    /** Scroll the view up one amount.  */
    void up();
    /** Scroll the view down one amount.  */
    void down();
    /** Scroll the view up one block (half screen/bounds size).  */
    void upBlock();
    /** Scroll the view down one block (half screen/bounds size).  */
    void downBlock();

    /** Set if the child layout should be stretched horizontally to fill the space available.
     * @param s the flag to use for setting the stretchiness.
     */
    void setStretchChildren(bool s=true);

    /** Scroll to a "percentage" (0 = 0%, 256 = 100%) of the screen.
     * @param value the percentage of the screen to scroll to, represented in fixed point.
     */
    void scrollToPercent(int value);

    /** Set this to be the top level ScrollPane. The top level pane is special
     * as its scroll bar does not fill the whole height, but is shown only on
     * the bottom screen. The top level ScrollPane also handles the pop-up menu.
     * @param topLevel true if this is the top level scroll pane. false
     * otherwise. ScrollPanes are initialised to not be the top level one.
     */
    void setTopLevel(bool topLevel=true);

    /** Get the current top level state.
     * @return true if this is the top level ScrollPane.
     */
    bool topLevel() const;

    /** Set the background fill colour shown between child Components.
     * @param color the color to use.
     */
    void setBackgroundColor(unsigned short color);

    // From Component
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);
    virtual void setSize(unsigned int w, unsigned int h);
    virtual void setLocation(unsigned int x, unsigned int y);

  private:
    static Component * s_popup;
    int m_scrollIncrement;
    bool m_topLevel;
    bool m_canScrollUp;
    bool m_canScrollDown;
    ScrollBar * m_scrollBar;
    unsigned short m_backgroundColour;
    bool m_stretchChildren;

    void layoutChildren();
    void calculateScrollBar();

    void showScrollBar(const nds::Rectangle & clip);
    void adjustScroll(int & scrollIncrement);
    void adjustScrollUp(int & scrollIncrement);
};

#endif
