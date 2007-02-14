#include "ndspp.h"
#include "Interrupt.h"
#include "ExamineBoard.h"
#include "Arena.h"

using namespace nds;

void ExamineBoard::show()
{
  Interrupt::disable();
  Arena::instance().display();
#if 0
  set_border_col(0);
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
