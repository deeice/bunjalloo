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
#include "libnds.h"
#include "ndspp.h"
#include "Arena.h"
#include "CreatePlayers.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Options.h"
#include "Splash.h"
#include "Text16.h"

using namespace nds;

static const int START_TEXT_POS_X(7);
static const int START_TEXT_POS_Y(17);
static const int OPTION_TEXT_POS_X(15);
static const int OPTION_TEXT_POS_Y(17);
static const int START_STR_LENGTH(5);
static const int OPTIONS_STR_LENGTH(7);

Splash::Splash()
{
  Rectangle initial(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
  Rectangle start(START_TEXT_POS_X*8,START_TEXT_POS_Y*8,
                     START_STR_LENGTH*8,16);
  Rectangle option(OPTION_TEXT_POS_X*8,OPTION_TEXT_POS_Y*8,
                     OPTIONS_STR_LENGTH*8,16);
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
    textBg.print("START",    START_TEXT_POS_X, START_TEXT_POS_Y, 12);
    textBg.print("OPTIONS", OPTION_TEXT_POS_X, OPTION_TEXT_POS_Y, 13);
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
