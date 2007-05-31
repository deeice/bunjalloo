#include "libnds.h"
#include "ndspp.h"
#include "Splash.h"
#include "Graphics.h"
#include "Arena.h"
#include "Text16.h"
#include "Misc.h"
#include "GameState.h"
#include "CreatePlayers.h"
#include "Options.h"
#include "HotSpot.h"

using namespace nds;

Splash::Splash()
{
  Rectangle initial = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
  Rectangle start = {7*8,17*8,5*8,16};
  Rectangle option = {15*8,17*8,5*8,16};
  m_hotspots.push_back(new HotSpot(initial, showMenuCb, this));
  m_hotspots.push_back(new HotSpot(start, startCb, this));
  m_hotspots.push_back(new HotSpot(option, optionCb, this));
}

void Splash::show()
{
  Graphics::clearPalettes();
  Text16::instance().clear();
  Arena::instance().clear();
  Graphics::instance().setAnimationParams(31,-8);
  m_menuOn = 0;
  m_hilightItem = 0;
  Text16 & textBg = Text16::instance();
  textBg.setColour(1, Color(31,0,31));
  textBg.print("CHAOS -THE BATTLE OF WIZARDS", 2, 2, 1);
  textBg.print("By Julian Gollop", 7,4, 2);  
  
  textBg.setColour(3, Color(0,30,30));
  textBg.print("NDS version" , 9,10, 3);  
  textBg.print("by Quirky", 10, 12, 3);
  
  textBg.setColour(12, Color(31,31,0));
  textBg.print("Press START", 9,17, 12);  
  m_animationPalette = 12;
  Arena::instance().decorativeBorder(15, Color(31,0,0),Color(31,0,31)); 
  Video::instance(0).fade(false);
}

void Splash::animate()
{
  Misc::churnRand();
  Color c(31,31,0);
  Graphics::instance().animateSelection(m_animationPalette, c);
}

void Splash::selectItem(int item) {
  m_animationPalette = 12 + item;
  Text16::instance().setColour(m_animationPalette, Color(31,31,0));
  Text16::instance().setColour(13-item, Color(20,20,0));
}

void Splash::handleKeys()
{
  u16 keysSlow = keysDownRepeat();

  if (keysSlow & KEY_LEFT) {
    left();
  } 
  if (keysSlow & KEY_RIGHT) {
    right();
  } 
  if (keysSlow & KEY_A) {
    a();
  } 
  if (keysSlow & KEY_START) {
    start();
  } 
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  }
}

void Splash::optionCb(void * arg)
{
  Splash * self = (Splash*)arg;
  if (self->m_menuOn)
  {
    self->m_hilightItem = 1;
    self->a();
  }
}

void Splash::startCb(void * arg)
{
  Splash * self = (Splash*)arg;
  if (self->m_menuOn)
  {
    self->start();
  }
}

void Splash::showMenuCb(void * arg)
{
  Splash * self = (Splash*)arg;
  if (not self->m_menuOn)
  {
    self->start();
  }
}

// start key pressed, for now start the game
void Splash::start(void) {
  if (not m_menuOn) {
    m_menuOn = true;
    m_hilightItem = 0;
    Text16 & textBg = Text16::instance();
    textBg.print("                    ", 2,17, 12);
    textBg.print("START",    7, 17, 12);
    textBg.print("OPTIONS", 15, 17, 13);
    selectItem(0);
  }
  else {
    Video::instance().fade();
    selectItem(0);
    //show_create_players();
    GameState::instance().setNextScreen(new CreatePlayers());
  }
}

void Splash::right(void) {
  if (m_menuOn == 0)
    return;
    
  if (m_hilightItem == 0)  {
    m_hilightItem = 1;
    selectItem(m_hilightItem);
  }
  
}

void Splash::left(void) {
  if (m_menuOn == 0)
    return;
  
  if (m_hilightItem == 1) {
    m_hilightItem = 0;
    selectItem(m_hilightItem);
  }
  
}

void Splash::a(void) {
  if (m_menuOn == 0)
    return;
  
  if (m_hilightItem == 1) {
    Video::instance().fade();
    //show_options();
    GameState::instance().setNextScreen(new OptionScreen());
  } else {
    Video::instance().fade();
    GameState::instance().setNextScreen(new CreatePlayers());
  }
}
