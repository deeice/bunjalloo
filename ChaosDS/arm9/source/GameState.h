#ifndef GameState_h_seen
#define GameState_h_seen

#include "ScreenI.h"

class GameState
{
  public:
    static GameState & instance();
    // checks if animation is required and, if it is, animates.
    // Or rather, calls the Arena routine that updates the arena values
    void checkVblankAnimation();
    inline void incrFrames();
    inline int gameFrame() const;
    inline CurrentScreen_t currentScreen() const;

    // set the next screen to show - safe to call in a handleKey callback
    inline void nextScreen(ScreenI * c);
    // execute a frame - calls handle keys and animate on current screen
    void mainLoopExecute();

  private:
    unsigned char m_gameFrames;
    ScreenI * m_currentScreen;
    ScreenI * m_nextScreen;

    GameState();
    GameState(const GameState&);
    inline void currentScreen(ScreenI * c);
};
#include "GameState.inl"

#endif
