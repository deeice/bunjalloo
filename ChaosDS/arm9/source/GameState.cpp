#include "GameState.h"
#include "SoundEffect.h"
#include "System.h"

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
  if (not m_currentScreen) 
    return;
  m_currentScreen->vblank();
}

void GameState::mainLoopExecute()
{
  if (m_nextScreen != 0) {
    this->currentScreen(m_nextScreen);
  }
  
  nds::System::checkSleep();

  m_currentScreen->handleKeys();
  if (m_gameFrames&1)
    m_currentScreen->animate();
}
