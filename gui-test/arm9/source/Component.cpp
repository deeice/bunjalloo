#include <algorithm>
#include "Component.h"

static const int NO_PREFERRED_SIZE(-1);

static void deleteChild(Component * child)
{
  delete child;
}

Component::Component() : 
  m_bounds(),
  m_preferredWidth(NO_PREFERRED_SIZE),
  m_preferredHeight(NO_PREFERRED_SIZE) 
{
  m_bounds.x = 0;
  m_bounds.y = 0;
  m_bounds.w = 0;
  m_bounds.h = 0;
}

Component::~Component()
{
  for_each(m_children.begin(), m_children.end(), deleteChild);
}

void Component::add(Component * child)
{
  // check if not already added
  std::vector<Component*>::const_iterator it = std::find(m_children.begin(), m_children.end(), child);
  if (it == m_children.end())
  {
    m_children.push_back(child);
  }
}

void Component::paint(const nds::Rectangle & area)
{
  std::vector<Component*>::iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    (*it)->paint(area);
  }
}

void Component::setSize(unsigned int w, unsigned int h)
{
  m_bounds.w = w;
  m_bounds.h = h;
}

nds::Rectangle Component::bounds() const
{
  return m_bounds;
}

nds::Rectangle Component::preferredSize() const
{
  nds::Rectangle r = {0, 0, m_preferredWidth, m_preferredHeight};
  if (m_preferredWidth == NO_PREFERRED_SIZE)
  {
    r.w = m_bounds.w;
  }
  if (m_preferredHeight == NO_PREFERRED_SIZE)
  {
    r.h = m_bounds.h;
  }
  return r;
}

void Component::setLocation(unsigned int x, unsigned int y)
{
  m_bounds.x = x;
  m_bounds.y = y;
}
