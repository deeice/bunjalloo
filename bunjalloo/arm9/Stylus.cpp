#include "Stylus.h"


Stylus::Stylus()
{
  m_state = CANCEL;
}
Stylus::~Stylus()
{
}

void Stylus::reset()
{
  m_state = CANCEL;
}

Stylus::ClickType Stylus::clickType() const
{
  return m_state;
}
void Stylus::startPoint(int & x, int & y) const
{
  x = m_startX;
  y = m_startY;
}
void Stylus::endPoint(int & x, int & y) const
{
  x = m_lastX;
  y = m_lastY;
}
void Stylus::update(bool touch, int x, int y)
{
  if (touch)
  {
    m_startX = x;
    m_startY = y;
    m_state = CLICK;
  }
  else
  {
    m_lastX = x;
    m_lastX = y;
  }
}
