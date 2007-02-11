
inline void GameState::incrFrames()
{
  m_gameFrames++;
}
inline int GameState::gameFrame() const
{
  return m_gameFrames;
}

inline CurrentScreen_t GameState::currentScreen() const
{
  return m_currentScreen->getId();
}
inline void GameState::currentScreen(ScreenI * c)
{
  if (m_currentScreen)
    delete m_currentScreen;
  m_currentScreen = c;
  m_currentScreen->show();
}

inline void GameState::nextScreen(ScreenI * c)
{
  if (m_nextScreen) {
    delete m_nextScreen;
    m_nextScreen = 0;
  }
  m_nextScreen = c;
}
