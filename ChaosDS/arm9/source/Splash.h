#ifndef Splash_h_seen
#define Splash_h_seen
#include "ScreenI.h"

class Splash: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_animationPalette;
    bool m_menuOn;
    int m_hilightItem;

    // keypad handling
    void left();
    void right();
    void a();
    void start();

    void selectItem(int item);
};
#endif
