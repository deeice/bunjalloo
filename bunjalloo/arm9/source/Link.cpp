#include "Link.h"
#include "HtmlElement.h"
#include "Rectangle.h"

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
std::string Link::href() const
{
  return unicode2string(m_anchor->attribute("href"));
}
