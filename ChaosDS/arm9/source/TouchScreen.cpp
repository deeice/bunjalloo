#include <algorithm>
#include "libnds.h"
#include "TouchScreen.h"
#include "HotSpot.h"

static void rmHS(HotSpot * hs)
{
  delete hs;
}
TouchScreen::~TouchScreen() 
{
  std::for_each(m_hotspots.begin(), m_hotspots.end(), rmHS);
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
