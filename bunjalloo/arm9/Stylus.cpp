#include "Stylus.h"

const static int HOLD_MAX(2000);
const int Stylus::HOLD_LIMIT(60);

Stylus::Stylus() :
  m_startX(0),
  m_startY(0),
  m_lastX(0),
  m_lastY(0),
  m_holding(0),
  m_state(WAITING)
{
}

Stylus::~Stylus()
{
}

void Stylus::reset()
{
  m_state = WAITING;
  m_holding = 0;
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
    if (m_state == WAITING)
    {
      // still holding down after reset - wait for let go
      return;
    }
    // if this is first time through, store start pos
    if (m_holding == 0)
    {
      m_startX = x;
      m_startY = y;
    }
    m_holding++;
    if (m_holding > HOLD_MAX)
    {
      m_holding = HOLD_MAX;
    }
    if (m_holding > HOLD_LIMIT)
    {
      m_state = HELD;
    }
    else
    {
      m_state = WAFFLE;
    }
    m_lastX = x;
    m_lastY = y;
  }
  else
  {
    if (m_state == WAITING)
    {
      // still holding down after reset - wait for let go
      m_state = CANCEL;
      return;
    }
    if (m_holding and (m_holding < HOLD_LIMIT))
    {
      m_state = CLICK;
    }
    else
    {
      m_state = CANCEL;
      m_holding = 0;
    }
  }
}
