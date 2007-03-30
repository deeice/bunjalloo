#include "System.h"
#include "libnds.h"

void nds::System::checkSleep()
{
  if (keysDown() & KEY_LID)
  {
    // remove current interrupts, set the lid open interrupt handler
#if 0
    powerOFF(POWER_LCD);
    swiIntrWait(1, IRQ_VBLANK);
    // wait for vblank before powering on the LCD
    while (REG_VCOUNT!=0);
    while (REG_VCOUNT==0);
    while (REG_VCOUNT!=0);
    powerON(POWER_LCD);
#endif
  }
}
