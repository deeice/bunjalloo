#include "Interrupt.h"
#include "GameState.h"

void Interrupt::processVblank()
{
  GameState::instance().incrFrames();
  /*
  GameState::instance().checkVblankAnimation();
  */
}

