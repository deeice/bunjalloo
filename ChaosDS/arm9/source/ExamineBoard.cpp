#include <nds.h>
#include <stdio.h>
#include "ndspp.h"
#include "Interrupt.h"
#include "ExamineBoard.h"
#include "ExamineSquare.h"
#include "GameState.h"
#include "GameMenu.h"
#include "Arena.h"

using namespace nds;
ExamineBoard::~ExamineBoard()
{
  // iprintf("~ExamineBoard;\n");
}

void ExamineBoard::show()
{
  Interrupt::disable();
  Arena & theArena(Arena::instance());
  theArena.display();
  theArena.setBorderColour(0);
  theArena.initialiseCursor(7,4);
  Interrupt::enable();
  Video::instance().fade(false);
}
void ExamineBoard::animate()
{}
CurrentScreen_t ExamineBoard::screenId() const
{
  return SCR_EXAMINE_BOARD;
}
void ExamineBoard::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
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
  if (keysSlow & KEY_RIGHT) {
    arena.cursorRight();
  }
  if (keysSlow & KEY_A) {
    a();
  }
  if (keysSlow & KEY_B) {
    b();
  }
}

void ExamineBoard::a() 
{
  int theCreature = Arena::instance().cursorContents();
  // return instantly if we examine an empty square
  if (theCreature == 0) {
    return;
  }
  Video::instance().fade();
  GameState::instance().setNextScreen(new ExamineSquare(new ExamineBoard()));
  Arena::instance().enableCursor(false);
}

void ExamineBoard::b() {
  Video::instance().fade();
  GameState::instance().setNextScreen(new GameMenu());
  Arena::instance().enableCursor(false);
}
