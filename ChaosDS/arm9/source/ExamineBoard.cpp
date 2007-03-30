#include "libnds.h"
#include "ndspp.h"
#include "Interrupt.h"
#include "ExamineBoard.h"
#include "ExamineSquare.h"
#include "GameState.h"
#include "GameMenu.h"
#include "Arena.h"
#include "Line.h"
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
  Video::instance().fade(false);
}
void ExamineBoard::animate()
{
  Arena::instance().drawCreatures();
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
    a();
  }
  if (keysSlow & KEY_B) {
    b();
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

void ExamineBoard::a() 
{
  int theCreature = Arena::instance().cursorContents();
  // return instantly if we examine an empty square
  if (theCreature == 0) {
    return;
  }
  Video::instance().fade();
  GameState::instance().setNextScreen(new ExamineSquare(new ExamineBoard(false)));
  Arena::instance().enableCursor(false);
}

void ExamineBoard::b() {
  Video::instance().fade();
  GameState::instance().setNextScreen(new GameMenu());
  Arena::instance().enableCursor(false);
}
