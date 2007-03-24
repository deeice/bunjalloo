#include <nds.h>
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

using namespace nds;

VictoryScreen::VictoryScreen(Victory_t winOrDraw):
  m_winOrDraw(winOrDraw),m_highlightItem(0),
  m_cycleColour1(0),
  m_cycleColour2(4),
  m_cycleColour3(8),
  m_cycleFrame(0)

{}

void VictoryScreen::show()
{
  if (m_winOrDraw == WIN)
  {
    win();
  }
  else
  {
    draw();
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

void VictoryScreen::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  if (keysSlow & KEY_A){
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
}

// Only CPU left, ask if we should carry on...
void VictoryScreen::endGameOption()
{
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
  text16.print("YES", 10,8, 7);  
  text16.print("NO",  16,8, 8);  
  
  text16.print("PRESS A", 11,16,  9);  
  
  text16.setColour(7, Color(31,31,0)); 
  text16.setColour(8, Color(12,12,0)); 
  
  bool decided(false);
  int highlightItem = 0;
  Video::instance().fade(false);
  while (not decided) {
    Misc::delay(2,false);
    scanKeys();
    u16 keysSlow = keysDownRepeat();
    
    if (keysSlow & KEY_LEFT) {
      highlightItem = 0;
    }
    if (keysSlow & KEY_RIGHT) {
      highlightItem = 1;
    }
    if (keysSlow & KEY_A) {
      decided = true;
    }
    
    if (keysSlow & KEY_B) {
      decided = true;
      highlightItem = 0;
    }
    
    if (highlightItem  == 0 ) {
      text16.setColour(7, Color(31,31,0)); 
      text16.setColour(8, Color(12,12,0)); 
    } else {
      text16.setColour(8, Color(31,31,0)); 
      text16.setColour(7, Color(12,12,0)); 
    }
  }
  arena.setCurrentPlayer(0);
  if (highlightItem) {
    arena.setCurrentPlayer(9);
  }
}
