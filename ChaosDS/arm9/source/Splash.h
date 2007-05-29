#ifndef Splash_h_seen
#define Splash_h_seen
#include "TouchScreen.h"

class Splash: public TouchScreen
{
  public:
    Splash();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_animationPalette;
    bool m_menuOn;
    int m_hilightItem;

    static void optionCb(void * arg);
    static void startCb(void * arg);
    static void showMenuCb(void * arg);

    // keypad handling
    void left();
    void right();
    void a();
    void start();

    void selectItem(int item);
};
#endif
