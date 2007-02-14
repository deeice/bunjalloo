#include "ndspp.h"
#include "Interrupt.h"
#include "ExamineBoard.h"
#include "Arena.h"

using namespace nds;

void ExamineBoard::show()
{
  Interrupt::disable();
  Arena & theArena(Arena::instance());
  theArena.display();
  theArena.setBorderColour(0);
  theArena.initialiseCursor(7,4);

#if 0
  cursor_x = 7;
  cursor_y = 4;
  draw_cursor(CURSOR_NORMAL_GFX);
  SET_CURSOR_POSITION(cursor_x, cursor_y, 0,0);
  redraw_cursor();
#endif
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
{}
