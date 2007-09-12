/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

const char * nds::System::uname()
{
  return "SDL";
}
