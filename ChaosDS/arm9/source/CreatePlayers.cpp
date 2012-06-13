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
#include <algorithm>
#include <functional>
#include "libnds.h"
#include "ndspp.h"

#include "Arena.h"
#include "CreatePlayers.h"
#include "EditName.h"
#include "GameMenu.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Rectangle.h"
#include "SoundEffect.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;
static const int PLAYER_WIZ_Y(2);
static const int PLAYER_WIZ_X(1);
static const int PLAYER_WIZ_NAME_X(6);
static const int PLAYER_WIZ_LEVEL_X(16);
static const int HOW_MANY_NUM_POS_X(22);
static const int HOW_MANY_NUM_POS_Y(1);
static const int START_POS_X(10);
static const int START_POS_Y(20);

CreatePlayers::CreatePlayers(bool start)
  : m_start(start)
{
  // Increase/decrease players
  Rectangle decrPlayerRect((HOW_MANY_NUM_POS_X-1)*8, HOW_MANY_NUM_POS_Y*8, 24, 16 );
  Rectangle incrPlayerRect((HOW_MANY_NUM_POS_X+3)*8, HOW_MANY_NUM_POS_Y*8, 24, 16 );
  m_hotspots.push_back(new HotSpot(decrPlayerRect, decrPlayerCb, this));
  m_hotspots.push_back(new HotSpot(incrPlayerRect, incrPlayerCb, this));

  // adjust player icons - have a giant rectangle that covers the icons
  int rectHeight(16*8);
  Rectangle iconRect(16, 24, 8, rectHeight );
  m_hotspots.push_back(new HotSpot(iconRect, changeIconCb, this));
  Rectangle iconColRect(16+8, 24, 8, rectHeight );
  m_hotspots.push_back(new HotSpot(iconColRect, changeIconColCb, this));
  // name rect - width is the width of the name (start of where level is drawn to start of name)
  Rectangle nameRect(PLAYER_WIZ_NAME_X*8, 24, 8*(PLAYER_WIZ_LEVEL_X-PLAYER_WIZ_NAME_X), rectHeight );
  m_hotspots.push_back(new HotSpot(nameRect, changeNameCb, this));
  // level rect
  Rectangle levelRect(PLAYER_WIZ_LEVEL_X*8, 24, 8*9, rectHeight );
  m_hotspots.push_back(new HotSpot(levelRect, changeLevelCb, this));

  Rectangle resetRect(4*8, HOW_MANY_NUM_POS_Y*8, 17*8, 16 );
  m_hotspots.push_back(new HotSpot(resetRect, resetPlayersCb, this));

  Rectangle startRect(START_POS_X*8, START_POS_Y*8, 11*8, 16);
  m_hotspots.push_back(new HotSpot(startRect, startCb, this));

}

// from IF ScreenI
void CreatePlayers::show()
{
  Text16::instance().clear();
  Graphics::clearPalettes();
  Graphics::loadPalette(9,9);
  if (m_start) {
    Arena::instance().setPlayers(2);
  }
  m_hilightItem = 0;
  Graphics::instance().setAnimationParams(30,-8);

  m_hilightWizard = 0;

  Text16 & text16 = Text16::instance();
  Palette p(0,10);
  p[0] = 0;
  text16.setColour(10, Color(31,30,30));
  text16.print("How many players?", 4, 1, 10);

  if (m_start) {
    // create the default start wizards
    Wizard::createDefaultWizards();
    selectItem(0);
  } else {
    m_hilightItem = Arena::instance().currentPlayer()+1;
    selectItem(m_hilightItem);
  }

  Arena::instance().decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 

  updatePlayers();

  Video::instance().fade(false);
}

// from IF ScreenI
void CreatePlayers::animate()
{
  Color c(0,31,31);
  if (m_hilightItem == 0)
    Graphics::instance().animateSelection(10, c);
  else 
    Graphics::instance().animateSelection(m_hilightItem-1, c);
}

// from IF ScreenI
void CreatePlayers::handleKeys()
{
  u16 keysSlow = keysDownRepeat();

  if (keysSlow & KEY_UP) {
    up();
  } 
  if (keysSlow & KEY_DOWN) {
    down();
  } 
  if (keysSlow & KEY_LEFT) {
    left();
  } 
  if (keysSlow & KEY_RIGHT) {
    right();
  } 
  if (keysSlow & KEY_L) {
    l();
  } 
  if (keysSlow & KEY_R) {
    r();
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
// Other CreatePlayers functions ....
void CreatePlayers::incrPlayerCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  self->deselectItem();
  self->m_hilightItem = 0;
  self->right();
}
 
void CreatePlayers::decrPlayerCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  self->deselectItem();
  self->m_hilightItem = 0;
  self->left();
}
void CreatePlayers::changeIconCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  if ( self->selectWizFromY() ) {
    self->r();
  }
}
void CreatePlayers::changeIconColCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  if ( self->selectWizFromY() ) {
    self->l();
  }
}
void CreatePlayers::changeNameCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  if ( self->selectWizFromY() ) {
    self->a();
  }
}
void CreatePlayers::changeLevelCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  if ( self->selectWizFromY() ) {
    Wizard & player = Wizard::player(self->m_hilightItem-1);
    int level = player.level();
    if (level == 8) {
      player.setLevel(0);
      SoundEffect::play(SND_MENU);
      self->updatePlayers();
    }
    else {
      self->right();
    }
  }
}

void CreatePlayers::resetPlayersCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  self->deselectItem();
  self->m_hilightItem = 0;
  self->a();
}

void CreatePlayers::startCb(void * arg)
{
  CreatePlayers * self = (CreatePlayers*)arg;
  self->start();
}

bool CreatePlayers::selectWizFromY()
{
  int y = this->m_y - 8;
  int wizIndex = y/16;
  bool selected(false);
  if (wizIndex <= Arena::instance().players()) {
    deselectItem();
    m_hilightItem = wizIndex;
    selected = true;
    Misc::delay(4);
  }
  return selected;
}
 
void CreatePlayers::selectItem(int item) {
  Graphics::instance().setAnimationParams(-31,-8);
  Color c(31,30,30);
  if (item == 0)
    Text16::instance().setColour(10, c);
  else
    Text16::instance().setColour(item-1, c);
}

// deselects current m_hilightItem
void CreatePlayers::deselectItem() {
  Color c(0,30,30);
  if (m_hilightItem == 0)
    Text16::instance().setColour(10, c);
  else
    Text16::instance().setColour(m_hilightItem-1, c);
}

class DrawWizard: public std::unary_function<Wizard,bool> {
  private:
    int m_index;
    int m_hilightItem;
    Text16 & m_text16;

  public:
    //! Draw a wizard on the choices screen.
    DrawWizard(int highlightItem):m_index(0), 
                                  m_hilightItem(highlightItem), 
                                  m_text16(Text16::instance()) 
    { }
    
    // for each element in the player array...
    //! Draws the wizard.
    result_type operator() (argument_type & element) {
      element.updateColour();
      int ypos = m_index*2 + PLAYER_WIZ_Y;
      element.draw8(PLAYER_WIZ_X, ypos, 0);
      if (m_index == (m_hilightItem - 1)) {
        m_text16.setColour(m_index, Color(31,30,30));
      } 
      else {
        m_text16.setColour(m_index, Color(0,30,30));
      }
      ypos += 1;
      element.printNameAt(PLAYER_WIZ_NAME_X,   ypos);
      element.printLevelAt(PLAYER_WIZ_LEVEL_X, ypos);
      m_index++;
      return true;
    }
};

// This should go in Wizard or Arena.
// previously: update_players
void CreatePlayers::updatePlayers(void) 
{
  using std::for_each;
  // clear the arena and redraw it...
  Arena::instance().clear();
  Wizard * players = Wizard::players();
  int playerCount = Arena::instance().players();
  for_each(players, players+playerCount, DrawWizard(m_hilightItem));
  // Hide the name of those not shown..
  for (int i = playerCount; i < 8; i++)
  {
    Text16::instance().setColour(i, 0);
  }
  char str[30];
  Text16::int2a(playerCount, str);
  Text16::instance().setColour(10, Color(0,30,30));
  Text16::instance().print(str, HOW_MANY_NUM_POS_X+2,1, 10);

  str[0] = Text16::LEFT_ARROW_INDEX;
  str[1] = 0;
  Text16::instance().print(str, HOW_MANY_NUM_POS_X,HOW_MANY_NUM_POS_Y, 10);
  str[0] = Text16::RIGHT_ARROW_INDEX;
  Text16::instance().print(str, HOW_MANY_NUM_POS_X+4,HOW_MANY_NUM_POS_Y, 10);
  Text16::instance().setColour(12, Color(31,30,0));
  Text16::instance().print("Press Start", START_POS_X, START_POS_Y, 12);
}

void CreatePlayers::up(void) {
  // wait_vsync_int();
  deselectItem();
  Graphics::instance().setAnimationColour(30);
  if (m_hilightItem > 0)
    m_hilightItem--;
    
  selectItem(m_hilightItem);
}


void CreatePlayers::down(void) {
  // wait_vsync_int();
  deselectItem();
  Graphics::instance().setAnimationColour(30);
  if (m_hilightItem < Arena::instance().players())
    m_hilightItem++;
  selectItem(m_hilightItem);
}

void CreatePlayers::left(void) {
  if (m_hilightItem == 0) {
    int playerCount = Arena::instance().players();
    if (playerCount > 2) {
      playerCount--;
      Arena::instance().setPlayers(playerCount);
      SoundEffect::play(SND_MENU);
    }
  } else {
    Wizard & player = Wizard::player(m_hilightItem-1);
    if (player.isCpu())
    {
      int level = player.level();
      level--;
      player.setLevel(level);
      SoundEffect::play(SND_MENU);
    }
  }
  updatePlayers();
}

void CreatePlayers::right(void) {
  if (m_hilightItem == 0) {
    int playerCount = Arena::instance().players();
    if (playerCount < 8) {
      playerCount++;
      Arena::instance().setPlayers(playerCount);
      SoundEffect::play(SND_MENU);
    }
  } 
  else {
    Wizard & player = Wizard::player(m_hilightItem-1);
    int level = player.level();
    if (level < 8) {
      level++;
      player.setLevel(level);
      SoundEffect::play(SND_MENU);
    }
  }
  updatePlayers();
}

void CreatePlayers::l(void) {
  // change colour
  if (m_hilightItem ==0)
    return;
  Wizard & player = Wizard::player(m_hilightItem-1);
  if (player.colour() < 8) {
    player.setColour(player.colour()+1);
  }
  else {
    player.setColour(0);
  }
  updatePlayers();
}

void CreatePlayers::r(void) {
  // change image
  if (m_hilightItem ==0)
    return;
  Wizard & player = Wizard::player(m_hilightItem-1);
  if (player.image() < 7) {
    player.setImage(player.image()+1);
  }
  else {
    player.setImage(0);
  }
  updatePlayers();
}


void CreatePlayers::a() {
  if (m_hilightItem == 0) {
    // A pressed on the "How many players?" bit
    // reset the players!  
    Wizard::createDefaultWizards();
    Text16::instance().clear();
    Text16::instance().print("How many players?", 4,1, 10);
    Text16::instance().setColour(10, Color(0,30,30));
    updatePlayers();
  } else {
    // go to the edit name screen
    Arena::instance().setCurrentPlayer(m_hilightItem-1);
    Video::instance().fade();
    GameState::instance().setNextScreen(new EditName());
  }
    
}

void CreatePlayers::start(void) {
  SoundEffect::play(SND_CHOSEN);
  // was init_players 
  Wizard::initialisePlayers();
  Arena::instance().setCurrentPlayer(0);
  Arena::instance().resetRoundCount();
  // check if we need to show game menu...
  Wizard & player0 = Wizard::player(0);
  if (player0.isCpu()) 
    Arena::instance().setCurrentPlayer(Wizard::nextHuman());
  if (Arena::instance().currentPlayer() == 9) 
  {
    // there is no human player!
    GameMenu::continueGame();
  } else {
    Video::instance().fade();
    GameState::instance().setNextScreen(new GameMenu());
    // fade_up();
  }
}
