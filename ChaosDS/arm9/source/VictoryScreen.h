#ifndef VictoryScreen_h_seen
#define VictoryScreen_h_seen

#include "ScreenI.h"

class VictoryScreen: public ScreenI
{
  public:
    //!Types of possible end game.
    enum Victory_t {
      WIN,       //!< There was a winner.
      DRAW       //!< No winner, game drawn.
    };
    //! Start the end game screen.
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
    int m_cycleColour1;
    int m_cycleColour2;
    int m_cycleColour3;
    int m_cycleFrame;


    void win();
    void draw();
};
#endif
