#include "libnds.h"
#include "ndspp.h"
#include "VictoryScreen.h"
#include "Arena.h"
#include "Text16.h"
#include "Wizard.h"
#include "Misc.h"
#include "Graphics.h"
#include "Casting.h"
#include "GameState.h"
#include "Splash.h"
#include "HotSpot.h"

using namespace nds;

static const int YES_POS_X(10);
static const int YES_POS_Y(8);
static const int NO_POS_X(16);
static const int NO_POS_Y(8);

VictoryScreen::VictoryScreen(Victory_t winOrDraw):
  m_winOrDraw(winOrDraw),m_highlightItem(0),
  m_cycleColour1(0),
  m_cycleColour2(4),
  m_cycleColour3(8),
  m_cycleFrame(0)

{
  if (m_winOrDraw == CONTINUE)
  {
    // hostpots on continue
    Rectangle yes = {YES_POS_X*8, YES_POS_Y*8, 3*8, 16};
    Rectangle no = {NO_POS_X*8, NO_POS_Y*8, 2*8, 16};

    m_hotspots.push_back(new HotSpot(yes, continueYesCb, this));
    m_hotspots.push_back(new HotSpot(no, continueNoCb, this));

  }
  else
  {
    Rectangle screenRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    m_hotspots.push_back(new HotSpot(screenRect, exitCb, this));
  }
}

void VictoryScreen::show()
{
  switch (m_winOrDraw)
  {
    case WIN:
      win();
      break;
    case DRAW:
      draw();
      break;
    case CONTINUE:
      continueGame();
      break;
  }
}

void VictoryScreen::draw()
{
  // "THE CONTEST IS DRAWN BETWEEN"
  // then wipe all the flags and stuff and return to splash screen
  // clear arena screen...
  Video::instance().fade();
  Arena & arena = Arena::instance();
  Text16 & text16 = Text16::instance();
  arena.enableCursor(false);
  arena.clear();
  text16.clear();
  Graphics::instance().clearPalettes();
  
  arena.decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 
  
  text16.setColour(10, Color(30,31,0)); // yellow
  text16.print("THE CONTEST IS DRAWN BETWEEN", 1,1, 10);  
  int y = 3;
  int playerCount(arena.players());
  for (int i = 0; i < playerCount; i++) {
    if (not Wizard::player(i).isDead()) {
      Wizard::player(i).printNameAt(9,y,13);
      y+=2;
    }
  }
  Video::instance().fade(false);
}

void VictoryScreen::win()
{
  // <player>" IS THE WINNER" 
  // then wipe all the flags and stuff and return to splash screen
  Video::instance().fade();
  Arena & arena = Arena::instance();
  int winner = 0;
  int playerCount(arena.players());
  for (int i = 0; i < playerCount; i++) {
    if (not Wizard::player(i).isDead()) {
      winner = i;
      break;
    }
  }
  // clear arena screen...
  arena.enableCursor(false);
  arena.clear();
  Text16 & text16 = Text16::instance();
  text16.clear();
  Graphics::instance().clearPalettes();
  
  arena.decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 
  
  //char str[30];
  text16.setColour(10, Graphics::s_chaosColours[2]); 
  text16.setColour(12, Graphics::s_chaosColours[4]); 
  text16.setColour(13, Graphics::s_chaosColours[8]); 
  
  text16.print("THE WINNER IS:", 8,2, 10);  
  text16.print("++++++++++++++++", 7,6, 12);  
  text16.print("+", 7,8, 12);  text16.print("+", 22,8, 12);  
  text16.print("+", 7,10, 12);  text16.print("+", 22,10, 12);  
  Wizard::player(winner).printNameAt(10,10,13);
  text16.print("+", 7,12, 12);  text16.print("+", 22,12, 12);  
  text16.print("++++++++++++++++", 7,14, 12);  
  Video::instance().fade(false);
}

void VictoryScreen::animate()
{
  if (m_winOrDraw != CONTINUE) {
    if (m_cycleFrame == 4) {
      Text16 & text16 = Text16::instance();
      text16.setColour(10, Graphics::s_chaosColours[m_cycleColour1]); 
      text16.setColour(12, Graphics::s_chaosColours[m_cycleColour2]); 
      text16.setColour(13, Graphics::s_chaosColours[m_cycleColour3]); 

      Palette p(0,15);
      p[1] = Graphics::s_chaosColours[m_cycleColour2];
      p[2] = Graphics::s_chaosColours[m_cycleColour1];
      m_cycleFrame = 0;
      m_cycleColour1++; 
      m_cycleColour2++; 
      m_cycleColour3++;
      if (m_cycleColour1 > 8) m_cycleColour1=0;
      if (m_cycleColour2 > 8) m_cycleColour2=0;
      if (m_cycleColour3 > 8) m_cycleColour3=0;
    } else {
      m_cycleFrame++;
    }
  }
}

void VictoryScreen::handleKeys()
{
  if (m_winOrDraw == CONTINUE)
  {
    continueKeys();
  }
  else
  {
    u16 keysSlow = keysDownRepeat();
    if (keysSlow & KEY_A){
      exit();
    }
    if (keysSlow & KEY_TOUCH){
      handleTouch();
    }
  }
}

void VictoryScreen::continueNoCb(void * arg)
{
  ((VictoryScreen*)arg)->m_highlightItem = 1;
  ((VictoryScreen*)arg)->m_decided = true;
}

void VictoryScreen::continueYesCb(void * arg)
{
  ((VictoryScreen*)arg)->m_highlightItem = 0;
  ((VictoryScreen*)arg)->m_decided = true;
}

void VictoryScreen::exitCb(void * arg)
{
  ((VictoryScreen*)arg)->exit();
}

void VictoryScreen::exit()
{
  Video::instance().fade();
  Arena & arena = Arena::instance();
  arena.clearGameBorder();
  arena.reset();
  Wizard::resetPlayers();
  Casting::resetWorldChaos();
  arena.setCurrentPlayer(0);
  Wizard::resetDeadWizards();
  arena.setPlayers(0);
  Graphics::loadAllPalettes();
  GameState::instance().setNextScreen(new Splash());
}

// Only CPU left, ask if we should carry on...
void VictoryScreen::endGameOption()
{
  VictoryScreen * self = new VictoryScreen(CONTINUE);
  GameState::instance().setNextScreen(self);
  self->m_decided = false;
  while (not self->m_decided) {
    swiWaitForVBlank();
    scanKeys();
    GameState::instance().mainLoopExecute();
  }
  Arena::instance().setCurrentPlayer(0);
  if (self->m_highlightItem) {
    Arena::instance().setCurrentPlayer(9);
  }
}

void VictoryScreen::continueGame() {
  Video::instance().fade();
  // clear arena screen...
  Arena & arena(Arena::instance());
  Text16 & text16(Text16::instance());
  arena.enableCursor(false);
  arena.clear();
  text16.clear();
  Graphics::instance().clearPalettes();
  arena.decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 
  
  //char str[30];
  text16.setColour(9, Color(31,31,0));
  
  text16.print("CONTINUE THE BATTLE?", 5,4, 9);  
  text16.print("YES", YES_POS_X, YES_POS_Y, 7);  
  text16.print("NO",  NO_POS_X,  NO_POS_Y, 8);  
  
  text16.print("PRESS A", 11,16,  9);  
  
  text16.setColour(7, Color(31,31,0)); 
  text16.setColour(8, Color(12,12,0)); 
  m_decided = false;
  m_highlightItem = 0;
  Video::instance().fade(false);
}

void VictoryScreen::continueKeys()
{
  Misc::delay(2,false);
  scanKeys();
  u16 keysSlow = keysDownRepeat();

  if (keysSlow & KEY_TOUCH){
    handleTouch();
  }

  if (keysSlow & KEY_LEFT) {
    m_highlightItem = 0;
  }
  if (keysSlow & KEY_RIGHT) {
    m_highlightItem = 1;
  }
  if (keysSlow & KEY_A) {
    m_decided = true;
  }

  if (keysSlow & KEY_B) {
    m_decided = true;
    m_highlightItem = 0;
  }

  Text16 & text16(Text16::instance());
  if (m_highlightItem  == 0 ) {
    text16.setColour(7, Color(31,31,0)); 
    text16.setColour(8, Color(12,12,0)); 
  } else {
    text16.setColour(8, Color(31,31,0)); 
    text16.setColour(7, Color(12,12,0)); 
  }

}
