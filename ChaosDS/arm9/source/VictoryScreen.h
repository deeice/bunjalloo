#ifndef VictoryScreen_h_seen
#define VictoryScreen_h_seen

#include "ScreenI.h"

class VictoryScreen: public ScreenI
{
  public:
    enum Victory_t {
      WIN,
      DRAW
    };
    VictoryScreen(Victory_t type);
    virtual void show();
    virtual void animate();
    virtual void handleKeys();

    /** Call this when only cpu players remain. Asks the player if they want to
     * continue. */
    static void endGameOption();
  private:
    Victory_t m_winOrDraw;
    int m_highlightItem;
    void win();
    void draw();
};
#endif
