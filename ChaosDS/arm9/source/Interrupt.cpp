#include <nds/interrupts.h>
#include "Interrupt.h"
#include "GameState.h"

void Interrupt::processVblank()
{
  GameState::instance().incrFrame();
  /*
  GameState::instance().checkVblankAnimation();
  */
}
void Interrupt::enable()
{
  irqEnable(IRQ_VBLANK);
}
void Interrupt::disable()
{
  irqDisable(IRQ_VBLANK);
}
