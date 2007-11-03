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
#ifndef ScrollBar_h_seen
#define ScrollBar_h_seen

#include "Component.h"

class ScrollPane;

/** The vertical bar that is shown on scrolling widgets. Contains an arrow to
 * scroll up, one to scroll down and a handle that both represents the current
 * position and can be used to drag the scroll position.
 */
class ScrollBar: public Component
{

  public:

    /** Draw a small arrow pointing downwards.
     * @param x the on screen x position of the arrow.
     * @param y the on screen y position of the arrow.
     */
    static void drawDownArrow(int x, int y);

    /** Draw a small arrow pointing upwards.
     * @param x the on screen x position of the arrow.
     * @param y the on screen y position of the arrow.
     */
    static void drawUpArrow(int x, int y);

    /** Sets up the ScrollBar.*/
    ScrollBar();

    /** Set the total value that the bar represents.
     * @param total the total value that the bar represents.
     */
    void setTotal(int total);

    /** Set the current value that the scroll is at.
     * @param value the current scroll position.
     */
    void setValue(int value);

    /** Get the total value that the bar represents.
     * @return the total value that the bar represents.
     */
    int total() const;

    /** Get the current value that the scroll is at.
     * @return the current scroll position.
     */
    int value() const;

    /** The visible range in which the scrolling handle may move, not counting
     * the up and down arrows and any other decoration.
     * @return the visible range in pixels.
     */
    int visibleRange() const;

    /** Set the ScrollPane that this ScrollBar is related to.
     * @param scrollPane the parent ScrollPane.
     */
    void setScrollable(ScrollPane * scrollPane);

    // from Component
    virtual bool touch(int x, int y);
    virtual void paint(const nds::Rectangle & clip);

  private:
    int m_total;
    int m_value;
    int m_handleSize;
    int m_handlePosition;
    int m_handleHeld;
    ScrollPane * m_scrollPane;

    void calculateHandle();

};
#endif
