#include "Link.h"
#include "URI.h"
#include "HtmlElement.h"
#include "Rectangle.h"
#include "Canvas.h"
#include "Palette.h"

Link::Link(const HtmlElement * a)
  : m_anchor(a)
{ }

static void removeRectangle(Rectangle * r)
{
  delete r;
}

Link::~Link()
{
  for_each(m_clickZones.begin(), m_clickZones.end(), removeRectangle);
  m_clickZones.clear();
}

void Link::appendClickZone(int x, int y, int w, int h)
{
  Rectangle * rect = new Rectangle;
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  m_clickZones.push_front(rect);
}

bool Link::hitTest(int x, int y) const
{
  RectangleList::const_iterator it(m_clickZones.begin());
  for (; it != m_clickZones.end() ; ++it)
  {
    Rectangle * rect = *it;
    if ( rect->hit(x,y))
      return true;
  }
  return false;
}


void Link::highlight() const
{
  RectangleList::const_iterator it(m_clickZones.begin());
  for (; it != m_clickZones.end() ; ++it)
  {
    Rectangle * rect = *it;
    nds::Canvas::instance().drawRectangle(rect->x,rect->y,rect->w, rect->h, nds::Color(31,0,30));
  }
}

std::string Link::href() const
{
  // FIXME - what about #?
  UnicodeString href = m_anchor->attribute("href");
  // strip off the # part?
  // replace ' ' with %20?
  return unicode2string(href);
}
