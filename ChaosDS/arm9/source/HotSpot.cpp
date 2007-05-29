
#include "HotSpot.h"

HotSpot::HotSpot(nds::Rectangle & rect, 
    HotSpotCallback_t callback, 
    void * data)
: m_area(rect),
  m_callback(callback),
  m_data(data)
{
}
void HotSpot::hitTest(int x, int y)
{
  if (m_area.hit(x,y) and m_callback)
  {
    m_callback(m_data);
  }
}
