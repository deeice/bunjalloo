/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "libnds.h"
#include "ndspp.h"
#include "Arena.h"
#include "ExamineBoard.h"
#include "ExamineSquare.h"
#include "GameMenu.h"
#include "GameState.h"
#include "HotSpot.h"
#include "Interrupt.h"
#include "Line.h"
#include "Misc.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;

ExamineBoard::ExamineBoard(bool start):
  m_start(start)
{ }

void ExamineBoard::show()
{
  Interrupt::disable();
  Arena & arena(Arena::instance());
  arena.display();
  arena.setBorderColour(arena.currentPlayer());
  if (m_start) {
    arena.initialiseCursor(7,4);
  } else {
    arena.enableCursor();
    arena.cursorSet();
  }
  Interrupt::enable();
  Text16::instance().clearMessage();

  Video::instance().fade(false);
}
void ExamineBoard::animate()
{
  Arena::instance().drawCreatures();
  if (m_examineScreen)
  {
    m_examineScreen->animate();
  }
}

void ExamineBoard::vblank()
{
  Arena::instance().countdownAnim();
}
void ExamineBoard::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  u16 keys= keysDown();
  Arena & arena(Arena::instance());
  if (keysSlow & KEY_UP) {
    arena.cursorUp();
  }
  if (keysSlow & KEY_DOWN) {
    arena.cursorDown();
  }
  if (keysSlow & KEY_LEFT) {
    arena.cursorLeft();
  }

  if (keys & KEY_L) {
    arena.highlightTargetCreations();
  }

  if (keysSlow & KEY_RIGHT) {
    arena.cursorRight();
  }
  if (keysSlow & KEY_A) {
    examine();
  }
  if (keysSlow & KEY_B) {
    next();
  }
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  }
#if 0
  static Line::Line_t type(Line::ARROW);
  if (keysSlow & KEY_Y) {
    type = (Line::Line_t)(type + 1);
  }
  // debug line routine - too fast!
  if (keysSlow & KEY_X) {
    int startIndex(Arena::instance().startIndex());
    Arena::instance().enableCursor(false);
    Arena::instance().setStartIndex(
        Arena::instance().wizardIndex(Arena::instance().currentPlayer()));
    Line l(type);
    l.execute();

    Arena::instance().setStartIndex(startIndex);
    Arena::instance().enableCursor();
  }
#endif
}

void ExamineBoard::execute()
{
  examine();
}

void ExamineBoard::next() {
  Video::instance(1).fade();
  Video::instance().fade();
  GameState::instance().setNextScreen(new GameMenu());
  Arena::instance().enableCursor(false);
}
