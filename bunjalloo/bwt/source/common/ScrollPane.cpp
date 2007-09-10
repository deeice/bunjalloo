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

Component * ScrollPane::s_popup(0);
// TODO: make configurable.
const static int SCROLLER_WIDTH(6);
const static int MIN_PADDING(2);
const static int TOP_LEVEL_SIZE(192);
using std::min;
using std::max;
using nds::Rectangle;

ScrollPane::ScrollPane()
:
  m_scrollIncrement(1), 
  m_topLevel(false),
  m_canScrollUp(false),
  m_canScrollDown(false),
  m_scrollBar(new ScrollBar),
  m_backgroundColour(nds::Color(31,31,31)),
  m_stretchChildren(false)
{ 
  m_scrollBar->setScrollable(this);
  m_preferredWidth = nds::Canvas::instance().width();
}

void ScrollPane::setStretchChildren(bool s)
{
  m_stretchChildren = s;
}

ScrollPane::~ScrollPane()
{
  delete m_scrollBar;
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

void ScrollPane::layoutChildren()
{
  int childWidth = m_bounds.w - SCROLLER_WIDTH - 1;
  std::vector<Component*>::iterator it(m_children.begin());
  int yPos = m_bounds.top();
  int lastXPos = m_bounds.x;
  int lastYPos = m_bounds.y;
  int rowHeight = 0;
  // printf(" ******** layoutChildren ******* \n");
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    // printf("Child :");
    Rectangle childPrefSize(c->preferredSize());
    childPrefSize.x = c->x(); 
    childPrefSize.y = c->y(); 
    // printf("x %d y %d w %d h %d\n", childPrefSize.x, childPrefSize.y, childPrefSize.w, childPrefSize.h);
    if ( (childPrefSize.w + lastXPos) < childWidth )
    {
      c->setLocation(lastXPos, lastYPos);
      if (childPrefSize.h > rowHeight)
      {
        rowHeight = childPrefSize.h;
      }
      childPrefSize.x = lastXPos;
      yPos = lastYPos+rowHeight+MIN_PADDING;
    }
    else {
      c->setLocation(x()+childPrefSize.x, yPos);
      lastYPos = yPos;
      yPos += childPrefSize.h+MIN_PADDING;
      rowHeight = childPrefSize.h;
    }
    childPrefSize.x = c->x(); 
    childPrefSize.y = c->y(); 

    if (childWidth <= childPrefSize.w)
      c->setSize(childWidth, childPrefSize.h);
    else if (m_stretchChildren)
    {
      c->setSize(childWidth, childPrefSize.h);
    }
    else
      c->setSize(childPrefSize.w, childPrefSize.h);
    childPrefSize.w = c->width();
    childPrefSize.h = c->height();
    // printf("       x %d y %d w %d h %d\n", childPrefSize.x, childPrefSize.y, childPrefSize.w, childPrefSize.h);
    lastXPos = x()+childPrefSize.x+c->width()+MIN_PADDING;
  }
}

void ScrollPane::calculateScrollBar()
{
  int topY = m_children.front()->bounds().top();
  int botY = m_children.back()->bounds().bottom();
  // Set the total coverage of the scrollBar
  m_scrollBar->setTotal(botY-topY);
  m_scrollBar->setSize(SCROLLER_WIDTH, m_topLevel? TOP_LEVEL_SIZE: m_bounds.h);
  m_scrollBar->setLocation(m_bounds.right() - SCROLLER_WIDTH, m_topLevel? TOP_LEVEL_SIZE: m_bounds.top());
  m_scrollBar->setValue((m_topLevel?TOP_LEVEL_SIZE:m_bounds.top())-topY);

  // Recalculate the scrolling.
  m_canScrollUp = false;
  m_canScrollDown = false;
  int topLimit = m_scrollBar->y();
  int botLimit = m_bounds.bottom();
  nds::Rectangle bound(m_children.front()->bounds());
  if (bound.y < topLimit)
  {
    m_canScrollUp = true;
  }
  bound = m_children.back()->bounds();
  if ( (bound.bottom()) > botLimit)
  {
    m_canScrollDown = true;
  } 
}

void ScrollPane::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w,h);
  if (m_children.empty()) {
    return;
  }

  layoutChildren();
  calculateScrollBar();

}

void ScrollPane::up()
{
  if (not m_canScrollUp)
    return;
  int scrollIncrement = m_scrollIncrement;
  adjustScrollUp(scrollIncrement);
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    int newY = c->y() + scrollIncrement;
    c->setLocation(c->x(), newY);
  }
  calculateScrollBar();
}

void ScrollPane::down()
{
  if (not m_canScrollDown) {
    return;
  }

  int scrollIncrement = m_scrollIncrement;
  adjustScroll(scrollIncrement);

  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    int newY = c->y() - scrollIncrement;
    c->setLocation(c->x(), newY);
  }
  calculateScrollBar();
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

  if (m_scrollBar->height() < m_scrollBar->total()) {
    m_scrollBar->paint(clip);
  }
}

void ScrollPane::paint(const nds::Rectangle & clip)
{
  nds::Canvas::instance().setClip(clip);
  if (m_topLevel) {
    nds::Canvas::instance().fillRectangle(clip.x, clip.y, clip.w, clip.h, m_backgroundColour);
  }
  if (not visible()) {
    return;
  }


  if (s_popup != 0 and this == s_popup)
  {
    nds::Canvas::instance().fillRectangle(clip.x, clip.y, clip.w, clip.h, m_backgroundColour);
  }

  // work out the total size of the scroll pane
  showScrollBar(clip);
  nds::Rectangle realClip = clip;
  realClip.w -= m_scrollBar->width();

  // paint the child components
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    Rectangle thisClip(intersect(realClip, bounds));
    // if the bounds of the component are smaller than the scrollpane, clip to the component.
    if (thisClip.w == 0 and thisClip.h == 0)
      continue;
    c->paint(thisClip);
    nds::Canvas::instance().setClip(realClip);
  }

  if (m_topLevel and s_popup != 0) {
    Rectangle bounds(s_popup->bounds());
    Rectangle thisClip(intersect(realClip, bounds));
    s_popup->paint(thisClip);
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
  if (not visible())
    return false;
  if (m_topLevel and s_popup and s_popup->bounds().hit(x, y))
  {
    // do not process other events if we hit the pop-up menu.
    s_popup->touch(x, y);
    return true;
  }

  bool handled(false);
  bool scrollBarHit = false;
  if (m_scrollBar->touch(x, y))
  {
    handled = true;
    scrollBarHit = true;
  }

  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    if (not scrollBarHit or (scrollBarHit and not bounds.hit(x, y)))
    {
      if (c->touch(x, y))
      {
        handled = true;
      }
    }
  }
  return handled;
}

void ScrollPane::upBlock()
{
  int scrollIncrement = m_scrollIncrement;
  m_scrollIncrement = m_bounds.h/2;
  if (m_scrollIncrement < scrollIncrement)
  {
    m_scrollIncrement = scrollIncrement;
  }
  up();
  m_scrollIncrement = scrollIncrement;

}

void ScrollPane::downBlock()
{
  int scrollIncrement = m_scrollIncrement;
  m_scrollIncrement = m_bounds.h/2;
  if (m_scrollIncrement < scrollIncrement)
  {
    m_scrollIncrement = scrollIncrement;
  }
  down();
  m_scrollIncrement = scrollIncrement;
}

void ScrollPane::scrollToPercent(int i)
{
  // scroll to a percentage of the view...
  // 0 is the top
  // 100 is the bottom ( y of last child is at view height ).
  // 50 is in the middle 

  int topLimit = m_scrollBar->y();
  int total = m_scrollBar->total() - m_scrollBar->visibleRange();
  int y = 0;
  if (total > 0)
  {
    // otherwise total height would be less than the visible range - so move to
    // topLimit
    y = (total * i) / 256;
  }
  // find difference and move by it
  int currentTop = m_children.front()->bounds().top();
  int dy = currentTop - topLimit + y;

  m_scrollBar->setValue(y);
  adjustScroll(dy);
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    c->setLocation(c->x(), c->y() - dy);
    //y += c->height();
  }
  calculateScrollBar();


}

void ScrollPane::adjustScroll(int & scrollIncrement)
{
  if (m_children.empty())
  {
    scrollIncrement = 0;
    return;
  }

  int offSet = (m_children.back()->bounds().bottom() - scrollIncrement) - m_bounds.bottom();
  int top = m_children.front()->bounds().top() - bounds().top();
  if (offSet < 0 and (top <= 0 or m_topLevel))
  {
    // scrolled too far.
    // change scrollIncrement so that offset == 0
    scrollIncrement += offSet;
  }
}

void ScrollPane::adjustScrollUp(int & scrollIncrement)
{
  if (m_children.empty())
  {
    scrollIncrement = 0;
    return;
  }
  int top = m_children.front()->bounds().top();
  int scrollY = m_scrollBar->y();
  if ((top + scrollIncrement) > scrollY)
  {
    scrollIncrement = scrollY - top;
  }
}

void ScrollPane::setBackgroundColor(unsigned short color)
{
  m_backgroundColour = color;
}

void ScrollPane::setPopup(Component * popup)
{
  s_popup = popup;
}
void ScrollPane::removePopup(Component * popup)
{
  if (s_popup == popup)
  {
    s_popup = 0;
  }
}
