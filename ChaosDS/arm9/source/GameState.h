#ifndef GameState_h_seen
#define GameState_h_seen

#include "ScreenI.h"

class GameState
{
  public:
    //! fetch the unique instance of the GameState
    static GameState & instance();

    /*! @brief Checks if animation is required and, if it is, animates.  Or
     * rather, calls the Arena routine that updates the arena values
     */
    void checkVblankAnimation();
    //! increase the frame counter
    inline void incrFrame();
    //! fetch the current game frame
    inline int gameFrame() const;
    //! set the current game frame to 0
    inline void resetGameFrame();
    //! set the next screen to show - safe to call in a handleKey callback
    inline void setNextScreen(ScreenI * c);
    //! execute a frame - calls handle keys and animate on current screen
    void mainLoopExecute();

    //! @return the curent screen.
    inline ScreenI * currentScreen() const;

  private:
    unsigned char m_gameFrames;
    ScreenI * m_currentScreen;
    ScreenI * m_nextScreen;

    GameState();
    GameState(const GameState&);
    inline void currentScreen(ScreenI * c);
};

inline void GameState::incrFrame()
{
  m_gameFrames++;
}
inline int GameState::gameFrame() const
{
  return m_gameFrames;
}
inline void GameState::resetGameFrame()
{
  m_gameFrames = 0;
}

inline void GameState::currentScreen(ScreenI * c)
{
  if (m_currentScreen)
    delete m_currentScreen;
  m_currentScreen = c;
  m_nextScreen = 0;
  m_currentScreen->show();
}

inline void GameState::setNextScreen(ScreenI * c)
{
  if (m_nextScreen) {
    delete m_nextScreen;
    m_nextScreen = 0;
  }
  m_nextScreen = c;
}
inline ScreenI * GameState::currentScreen() const
{
  return m_currentScreen;
}

#endif
