#include <nds/interrupts.h>
#include "Interrupt.h"
#include "GameState.h"

static bool s_processFrames(true);
void Interrupt::processVblank()
{
  if (s_processFrames) {
    GameState::instance().incrFrame();
    GameState::instance().checkVblankAnimation();
  }
}
void Interrupt::enable()
{
  s_processFrames = true;
}
void Interrupt::disable()
{
  s_processFrames = false;
}
