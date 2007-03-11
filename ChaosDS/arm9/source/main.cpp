#include <nds.h>
#include <stdio.h>
#include "ndspp.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Arena.h"
#include "GameState.h"
#include "Splash.h"
#include "Text16.h"
#include "Options.h"

int main(void) {
  irqInit();
  irqSet(IRQ_VBLANK, Interrupt::processVblank);
  
  Graphics::initialiseScreen();
  nds::Video::instance(0).setFade(0);
  // debug...
  nds::Background bg(1,0,0,31);
  BG_PALETTE_SUB[255] = RGB15(31,31,31);
  consoleInitDefault(bg.mapData(), bg.tileData(),16);
  bg.enable();
  GameState::instance().setNextScreen(new Splash());
  while(not Options::instance().option(Options::QUIT)) {
    scanKeys();
    swiWaitForVBlank();
    GameState::instance().mainLoopExecute();
  }

  return 0;
}
