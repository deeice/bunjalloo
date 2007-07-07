#include "libnds.h"
#include <stdio.h>
#include "ndspp.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Arena.h"
#include "GameState.h"
#include "Splash.h"
#include "Text16.h"
//#include "Options.h"

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
    scanKeys();
    swiWaitForVBlank();
    GameState::instance().mainLoopExecute();
  }

  return 0;
}
