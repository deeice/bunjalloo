#include <nds.h>
#include "ndspp.h"
#include "Graphics.h"
#include "Interrupt.h"
#include "Arena.h"
#include "GameState.h"
#include "Splash.h"
#include "Text16.h"

int main(void) {
  irqInit();
  irqSet(IRQ_VBLANK, Interrupt::processVblank);
  
  Graphics::initialiseScreen();
  nds::Video::instance(0).setFade(0);
  //Arena::instance().reset();
  // debug...
  nds::Background bg(1,0,0,31);
  bg.enable();
  BG_PALETTE_SUB[255] = RGB15(31,31,31);
  consoleInitDefault(bg.mapData(), bg.tileData(),16);

  GameState::instance().setNextScreen(new Splash());
  while(1) {
    scanKeys();
    swiWaitForVBlank();
    GameState::instance().mainLoopExecute();
  }

  return 0;
}
