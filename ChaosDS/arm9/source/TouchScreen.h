#ifndef TouchScreen_h_seen
#define TouchScreen_h_seen

#include "ScreenI.h"
#include <list>

class HotSpot;
//! Abstract class that implements touch screen.
class TouchScreen: public ScreenI
{
  public:
    ~TouchScreen();

  protected:
    HotSpot * m_checking;
    int m_x;
    int m_y;
    std::list<HotSpot*> m_hotspots;
    //! Handle touch screen presses
    void handleTouch();
};
#endif
