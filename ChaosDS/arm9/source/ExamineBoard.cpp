#include <nds.h>
#include "ndspp.h"
#include "Interrupt.h"
#include "ExamineBoard.h"
#include "GameState.h"
#include "GameMenu.h"
#include "Arena.h"

using namespace nds;

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
CurrentScreen_t ExamineBoard::getId() const
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
#if 0
  // return instantly if we examine an empty square
  if (arena[0][index] == 0) {
    return;
  }
  s32 bgx = bg_level.x_scroll;
  s32 bgy = bg_level.y_scroll;
  examine_creature(index);
  fade_down();
  display_arena();
  SET_CURSOR_POSITION(cursor_x, cursor_y, bgx,bgy);
  clear_message();
  set_border_col(current_player);
  redraw_cursor();
  display_cursor_contents(index);
  fade_up();
#endif
}

void ExamineBoard::b() {
  Video::instance().fade();
  GameState::instance().nextScreen(new GameMenu());
  Arena::instance().removeCursor();
}
