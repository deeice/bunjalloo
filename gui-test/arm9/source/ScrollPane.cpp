#include "Canvas.h"
#include "Palette.h"
#include "ScrollPane.h"
#include <algorithm>

const static int SCROLLER_WIDTH(4);
using std::min;
using std::max;

ScrollPane::ScrollPane():m_scrollIncrement(1)
{ }

void ScrollPane::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w,h);
  // resize the scrolling pane
  // resize children too
  int childWidth = w - SCROLLER_WIDTH;
  std::vector<Component*>::iterator it(m_children.begin());
  int yPos = (*it)->y();
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    nds::Rectangle r(c->preferredSize());
    c->setSize(childWidth, r.h);
    c->setLocation(0, yPos);
    yPos += r.h;
  }
}

void ScrollPane::up()
{
  setSize(m_bounds.w, m_bounds.h);
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    c->setLocation(c->x(), c->y() + m_scrollIncrement);
  }
}

void ScrollPane::down()
{
  setSize(m_bounds.w, m_bounds.h);
  // move all up one unit...
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    c->setLocation(c->x(), c->y() - m_scrollIncrement);
  }
}

static nds::Rectangle intersect(const nds::Rectangle & r1, const nds::Rectangle & r2)
{
  bool haveIntersect =  not ( r1.right() < r2.left()
      or r1.left() > r2.right()
      or r1.top() > r2.bottom() 
      or r1.bottom() < r2.top()
      );
  nds::Rectangle rect = {0, 0, 0, 0};
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
  int totalHeight = 0;
  int topY(0);
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    nds::Rectangle bounds(c->bounds());
    if (it == m_children.begin()) {
      topY = bounds.y;
    }
    totalHeight += bounds.h;
  }
  if (clip.h < totalHeight) {
    int scrollBarX = m_bounds.x + m_bounds.w - SCROLLER_WIDTH;
    int scrollBarY = (m_bounds.y < 192) ? 192:m_bounds.y;
    int scrollerHeight = (m_bounds.y < 192) ? (m_bounds.h - 192) :m_bounds.h;
    printf("Scroll needed show at %d %d\n", scrollBarX, scrollBarY  );
    nds::Canvas::instance().fillRectangle(scrollBarX, scrollBarY, SCROLLER_WIDTH, scrollerHeight, nds::Color(31,0,31));

    // show a bead at the % of the scroller.
    printf("Total H %d\n", totalHeight);
    printf("Top y %d\n", topY);
    // top is scrollBarY 
    // bottom is scrollBarY + scrollerHeight
    // clip.y is 0 or if clip.y < 192, 0 is 192
    // ratio = scrollBarX
    nds::Canvas::instance().fillRectangle(scrollBarX, scrollBarY, SCROLLER_WIDTH, 5, nds::Color(31,31,31));
  }
}

void ScrollPane::paint(const nds::Rectangle & clip)
{
  nds::Canvas::instance().setClip(clip);
  nds::Canvas::instance().fillRectangle(clip.x, clip.y, clip.w, clip.h, 0);

  // work out the total size of the scroll pane
  showScrollBar(clip);

  // paint the child components
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    Component * c(*it);
    nds::Rectangle bounds(c->bounds());
    nds::Rectangle thisClip(intersect(clip, bounds));
    // if the bounds of the component are smaller than the scrollpane, clip to the component.
    if (thisClip.w == 0 and thisClip.h == 0)
      continue;
    //printf("Clip %d %d %d %d\n", thisClip.x, thisClip.y, thisClip.w, thisClip.h);
    c->paint(thisClip);
  }
}

void ScrollPane::setScrollIncrement(int scrollIncrement)
{
  m_scrollIncrement = scrollIncrement;
}
