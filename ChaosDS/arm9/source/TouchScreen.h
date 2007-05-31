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

    /** Get the last touch position as an X-Y tile position offset from the corner of the touch rectangle.
     * @param x Return value for the x tile position.
     * @param y Return value for the y tile position.
     */
    void lastTouchToTile16(int & x, int &y) const;

  protected:
    HotSpot * m_checking;
    int m_x;
    int m_y;
    std::list<HotSpot*> m_hotspots;
    //! Handle touch screen presses
    void handleTouch();
};
#endif
