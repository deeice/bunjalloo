#include "GameState.h"
#include "Arena.h"

GameState & GameState::instance()
{
  static GameState s_instance;
  return s_instance;
}

GameState::GameState():
  m_gameFrames(0),
  m_currentScreen(0),
  m_nextScreen(0)
{
}

void GameState::checkVblankAnimation() {
  CurrentScreen_t currentScreen = m_currentScreen->getId();
  if (currentScreen == SCR_EXAMINE_BOARD 
      or currentScreen == SCR_CASTING 
      or currentScreen == SCR_MOVEMENT) 
  {
    Arena::instance().countdownAnim();
  }
}

void GameState::mainLoopExecute()
{
  if (m_nextScreen != 0) {
    this->currentScreen(m_nextScreen);
    m_nextScreen = 0;
  }
  m_currentScreen->handleKeys();
  if (m_gameFrames&1)
    m_currentScreen->animate();
}
