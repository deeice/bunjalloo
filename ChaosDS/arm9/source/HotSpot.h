#ifndef HotSpot_h_seen
#define HotSpot_h_seen

#include "Rectangle.h"

typedef void (*HotSpotCallback_t)(void*);

class HotSpot
{
  public:
    HotSpot(nds::Rectangle & rect, HotSpotCallback_t callback, void * data=0);
    void hitTest(int x, int y);

  private:
    nds::Rectangle m_area;
    HotSpotCallback_t m_callback;
    void * m_data;
};
#endif
