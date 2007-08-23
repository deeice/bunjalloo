/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include <stdio.h>
#include "libnds.h"
#include "ndspp.h"
#include "Arena.h"
#include "GameState.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Splash.h"
#include "Text16.h"

int main(void) {
  irqInit();
  irqSet(IRQ_VBLANK, Interrupt::processVblank);
  
  Graphics::initialiseScreen();
  // debug...
  /*
  nds::Background bg(1,0,0,31);
  nds::Palette p(1);
  p[255] = nds::Color(31,31,31);
  // consoleInitDefault(bg.mapData(), bg.tileData(),16);
  bg.enable();
  // iprintf("Chaos DS\n");
  */
  GameState::instance().setNextScreen(new Splash());
  keysSetRepeat(30,10);
  while(/*not Options::instance().option(Options::QUIT)*/ true) {
    swiWaitForVBlank();
    scanKeys();
    GameState::instance().mainLoopExecute();
  }

  return 0;
}
