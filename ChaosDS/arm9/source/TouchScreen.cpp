#include <algorithm>
#include "libnds.h"
#include "HotSpot.h"
#include "TouchScreen.h"

static void rmHS(HotSpot * hs)
{
  delete hs;
}
TouchScreen::~TouchScreen() 
{
  clearHotSpots();
}
void TouchScreen::clearHotSpots()
{
  std::for_each(m_hotspots.begin(), m_hotspots.end(), rmHS);
  m_hotspots.clear();
}

void TouchScreen::handleTouch()
{
  touchPosition tp = touchReadXY();
  m_x = tp.px;
  m_y = tp.py;
  std::list<HotSpot*>::iterator it(m_hotspots.begin());
  for (; it != m_hotspots.end(); ++it)
  {
    HotSpot * spot(*it);
    m_checking = spot;
    spot->hitTest(m_x, m_y);
  }
}

void TouchScreen::lastTouchToTile16(int & x, int &y) const
{
  x = (m_x - m_checking->area().x)/16;
  y = (m_y - m_checking->area().y)/16;
}
