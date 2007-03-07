
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

inline CurrentScreen_t GameState::currentScreen() const
{
  return m_currentScreen->screenId();
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
