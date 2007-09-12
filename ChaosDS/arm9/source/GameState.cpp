/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
