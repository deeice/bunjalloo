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
#include "Canvas.h"
#include "Palette.h"
#include "ScrollPane.h"
#include "ScrollBar.h"
#include <algorithm>

// TODO: make configurable.
const static int SCROLLER_WIDTH(6);
const static int MIN_PADDING(6);
using std::min;
using std::max;
using nds::Rectangle;

ScrollPane::ScrollPane()
:
  m_scrollIncrement(1), 
  m_topLevel(false),
  m_canScrollUp(false),
  m_canScrollDown(false),
  m_scrollBar(new ScrollBar)
{ 
  m_scrollBar->setScrollable(this);
  m_preferredWidth = nds::Canvas::instance().width();
}

void ScrollPane::setLocation(unsigned int x, unsigned int y)
{
  int dx = x-m_bounds.x;
  int dy = y-m_bounds.y;
  Component::setLocation(x,y);
  Rectangle scrollBarBounds(m_scrollBar->bounds());
  m_scrollBar->setLocation(scrollBarBounds.x + dx, scrollBarBounds.y + dy);
  if (m_children.empty()) {
    return;
  }
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle r(c->bounds());
    c->setLocation(r.x + dx, r.y + dy);
  }
}

void ScrollPane::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w,h);
  if (m_children.empty()) {
    return;
  }

  // resize the scrolling pane
  // resize children too
  int childWidth = w - SCROLLER_WIDTH;
  std::vector<Component*>::iterator it(m_children.begin());
  int yPos = (*it)->y();
  int lastXPos = 0;
  int lastYPos = 0;
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle r(c->preferredSize());
    if ( (r.w + lastXPos) < m_bounds.w )
    {
      c->setLocation(lastXPos, lastYPos);
      r.x = lastXPos;
    }
    else {
      c->setLocation(r.x, yPos);
      lastYPos = yPos;
      yPos += r.h;
    }
    c->setSize(min(childWidth, r.w), r.h);
    lastXPos = r.x+c->width()+MIN_PADDING;
  }
  int topY = m_children.front()->bounds().top();
  // Set the total coverage of the scrollBar
  m_scrollBar->setTotal(yPos-topY);
  m_scrollBar->setSize(SCROLLER_WIDTH, m_topLevel? 192: m_bounds.h);
  m_scrollBar->setLocation(m_bounds.right() - SCROLLER_WIDTH, m_topLevel? 192: m_bounds.top());
  m_scrollBar->setValue((m_topLevel?192:m_bounds.top())-topY);

  // Recalculate the scrolling.
  m_canScrollUp = false;
  m_canScrollDown = false;
  int topLimit = m_topLevel?192:m_bounds.y;
  int botLimit = m_bounds.bottom();
  nds::Rectangle bound(m_children[0]->bounds());
  if (bound.y < topLimit)
  {
    m_canScrollUp = true;
  }
  if (m_children.size() > 1)
  {
    bound = m_children.back()->bounds();
  }
  if ( (bound.bottom()) > botLimit)
  {
    m_canScrollDown = true;
  } 

}

void ScrollPane::up()
{
  setSize(m_bounds.w, m_bounds.h);
  if (not m_canScrollUp)
    return;
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    int newY = c->y() + m_scrollIncrement;
    c->setLocation(c->x(), newY);
  }
}

void ScrollPane::down()
{
  setSize(m_bounds.w, m_bounds.h);
  if (not m_canScrollDown) {
    return;
  }
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    int newY = c->y() - m_scrollIncrement;
    c->setLocation(c->x(), newY);
  }
}

static nds::Rectangle intersect(const nds::Rectangle & r1, const nds::Rectangle & r2)
{
  bool haveIntersect =  not ( r1.right() < r2.left()
      or r1.left() > r2.right()
      or r1.top() > r2.bottom() 
      or r1.bottom() < r2.top()
      );
  Rectangle rect = {0, 0, 0, 0};
  if (not haveIntersect)
  {
    return rect;
  }
  rect.x = max(r1.left(), r2.left());
  rect.y = max(r1.top(), r2.top());
  int minRight = min(r1.right(), r2.right());
  rect.w = minRight-rect.x;
  int minBottom = min(r1.bottom(), r2.bottom());
  rect.h = minBottom-rect.y;

  return rect;
}

void ScrollPane::showScrollBar(const nds::Rectangle & clip)
{
  if (m_children.empty())
  {
    return;
  }

  if (clip.h < m_scrollBar->total()) {
    m_scrollBar->paint(clip);
  }
}

void ScrollPane::paint(const nds::Rectangle & clip)
{
  nds::Canvas::instance().setClip(clip);
  if (m_topLevel) {
    nds::Canvas::instance().fillRectangle(clip.x, clip.y, clip.w, clip.h, 0);
  }

  // work out the total size of the scroll pane
  showScrollBar(clip);

  // paint the child components
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    Rectangle thisClip(intersect(clip, bounds));
    // if the bounds of the component are smaller than the scrollpane, clip to the component.
    if (thisClip.w == 0 and thisClip.h == 0)
      continue;
    c->paint(thisClip);
  }
}

void ScrollPane::setScrollIncrement(int scrollIncrement)
{
  m_scrollIncrement = scrollIncrement;
}

int ScrollPane::scrollIncrement() const
{
  return m_scrollIncrement;
}

void ScrollPane::setTopLevel(bool topLevel)
{
  m_topLevel = topLevel;
}

bool ScrollPane::topLevel() const
{
  return m_topLevel;
}

bool ScrollPane::touch(int x, int y)
{
  if (m_scrollBar->touch(x, y))
  {
    return true;
  }

  bool handled(false);
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    if (c->touch(x, y))
    {
      handled = true;
    }
  }
  // whatever happens, stop touching.
  return handled;
}

void ScrollPane::upBlock()
{
  int scrollIncrement = m_scrollIncrement;
  m_scrollIncrement = scrollIncrement * 4;
  up();
  m_scrollIncrement = scrollIncrement;

}

void ScrollPane::downBlock()
{
  int scrollIncrement = m_scrollIncrement;
  m_scrollIncrement = scrollIncrement * 4;
  down();
  m_scrollIncrement = scrollIncrement;
}

void ScrollPane::scrollToPercent(int i)
{
  // scroll to a percentage of the view...
  // 0 is the top
  // 100 is the bottom ( y of last child is at view height ).
  // 50 is in the middle 

  int topLimit = m_topLevel?192:m_bounds.top();
  int total = m_scrollBar->total() - (m_topLevel?192:m_bounds.h);

  int y = topLimit - ( (total * i) / 100 );
  // find difference and move by it
  int currentTop = m_children.front()->bounds().top();
  int dy = currentTop - y;

  m_scrollBar->setValue((m_topLevel?192:m_bounds.top())-y);
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    c->setLocation(c->x(), c->y() - dy);
    //y += c->height();
  }


}




