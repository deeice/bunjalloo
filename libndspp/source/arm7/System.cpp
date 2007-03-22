#include "System.h"
#include <nds.h>

static void lidOpenHandler()
{
  writePowerManagement(PM_CONTROL_REG,
      PM_SOUND_PWR |
      //PM_SOUND_VOL | // this turns sound *off*
      PM_BACKLIGHT_BOTTOM |
      PM_BACKLIGHT_TOP |
      PM_LED_ON);
}

void nds::System::checkSleep()
{
  // if the lid is down:
  if (REG_KEYXY & (BIT(7)))
  {
    // save previous interrupts
    unsigned long oldIE = REG_IE;
    // remove current interrupts, set the lid open interrupt handler
    REG_IME = 0;
    irqSet(IRQ_LID, lidOpenHandler);
    irqDisable((IRQ_MASK)(oldIE));
    powerOFF(POWER_SOUND);

    // set the lid open interrupt on
    irqEnable(IRQ_LID);

    // make the light flicker, mostly off
    writePowerManagement(PM_CONTROL_REG, PM_LED_CONTROL(1));

    swiSleep();

    // restore interrupts
    irqDisable(IRQ_LID);
    irqEnable((IRQ_MASK)(oldIE));

    // now sleep is over
    powerON(POWER_SOUND);

  }
}

