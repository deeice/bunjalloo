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
#include "Casting.h"
#include "ExamineBoard.h"
#include "GameMenu.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Rectangle.h"
#include "SoundEffect.h"
#include "SpellSelect.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;
static const int MENU_XPOS(4);
static const int MENU_YPOS(8);
static const int CHAOS_GAUGE_YPOS(4);
static const int MAX_GAME_MENU(3);
static const int TOUCH_PAUSE(4);

GameMenu::GameMenu()
{
  int width = 20*8;
  Rectangle selectSpellRect = {MENU_XPOS*8, MENU_YPOS*8, width, 16+16+8 };
  m_hotspots.push_back(new HotSpot(selectSpellRect, selectSpellCb, this));

  Rectangle examineRect = {MENU_XPOS*8, (6+MENU_YPOS)*8, width, 16 };
  m_hotspots.push_back(new HotSpot(examineRect, examineCb, this));

  Rectangle continueRect = {MENU_XPOS*8, (9+MENU_YPOS)*8, width, 16 };
  m_hotspots.push_back(new HotSpot(continueRect, continueCb, this));
}

void GameMenu::show()
{
  m_hilightItem = 0;
  Text16 & text16 = Text16::instance();
  Arena & arena = Arena::instance();

  text16.clear();
  arena.clear();
  Graphics::instance().clearPalettes();

  text16.setColour(12, Color(30,31,0));
  Wizard::currentPlayer().printNameAt(4,2,12);
  
  arena.decorativeBorder(15, Color(31,0,0),Color(31,31,0)); 
  
  // draw chaos/law val - colour 13
  int worldChaos = Casting::worldChaos();
  if (worldChaos != 0) {
    int x = MENU_XPOS;
    text16.setColour(13, Color(31,30,0));
    
    if (worldChaos < 0) {
      // chaos
      text16.print("(CHAOS ", x,CHAOS_GAUGE_YPOS, 13); 
      x += 7;
      int A = -worldChaos / 4;
      if (A != 0) {
        for (int i = 0; i < A; i++) {
          text16.print("*", x++,CHAOS_GAUGE_YPOS, 13); 
        } 
      }
      
    } else {
      // law
      text16.print("(LAW ", x,CHAOS_GAUGE_YPOS, 13); 
      x += 5;
      int A = worldChaos / 4;
      if (A != 0) {
        for (int i = 0; i < A; i++) {
          text16.print("+", x++,CHAOS_GAUGE_YPOS, 13); 
        } 
      }
      
    }
    text16.print(")", x,CHAOS_GAUGE_YPOS, 13); 
    
  }
  
  deselectItem(0);
  deselectItem(1);
  deselectItem(2);
  deselectItem(3);
  
  selectItem();
  
  text16.print("1. EXAMINE SPELL", MENU_XPOS,MENU_YPOS, 0);  
  text16.print("2. SELECT SPELL", MENU_XPOS,MENU_YPOS+3, 1);  
  text16.print("3. EXAMINE BOARD", MENU_XPOS,MENU_YPOS+6, 2);  
  text16.print("4. CONTINUE WITH GAME", MENU_XPOS,MENU_YPOS+9, 3);
  // sleep mode automatic when closing the lid
  // text16.print("4. ENTER SLEEP MODE", MENU_XPOS,MENU_YPOS+9, 3);
  Video::instance().fade(false);
}
void GameMenu::animate()
{
}

void GameMenu::handleKeys()
{
  u16 keysSlow = keysDownRepeat();

  if (keysSlow & KEY_UP) {
    up();
  } 
  if (keysSlow & KEY_DOWN) {
    down();
  } 
  if (keysSlow & KEY_A) {
    a();
  } 
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  } 
}

void GameMenu::deselectItem(int item) {
  Text16::instance().setColour(item, Color(0,16,16));
}

void GameMenu::selectItem() {
  Text16::instance().setColour(m_hilightItem, Color(0,31,31));
}

void GameMenu::up() {
  deselectItem();
  if (m_hilightItem > 0) {
    m_hilightItem--;
    SoundEffect::play(SND_MENU);
  }
  selectItem();
}

void GameMenu::down() {
  deselectItem();
  if (m_hilightItem < MAX_GAME_MENU) {
    m_hilightItem++;
    SoundEffect::play(SND_MENU);
  }
  selectItem();
}

void GameMenu::continueGame() 
{
  Video::instance().fade();
  int nextPlayer = Wizard::nextHuman(Arena::instance().currentPlayer());
  if (nextPlayer == 9) {
    // continue game after spell selection... this is roughly at 95c7
    GameState::instance().setNextScreen(new Casting());
  } else {
    Arena::instance().setCurrentPlayer(nextPlayer);
    GameState::instance().setNextScreen(new GameMenu());
  }
}

void GameMenu::a() {
  // get the hilited item...
  SoundEffect::play(SND_CHOSEN);
  bool isExamine(false);
  switch (m_hilightItem) 
  {
    case 0: 
      // examine spells
      isExamine = true;
    case 1: 
      // select or examine spells
      Video::instance().fade();
      GameState::instance().setNextScreen(new SpellSelect(isExamine));
      break;
    case 2: 
      // view arena
      Video::instance().fade();
      GameState::instance().setNextScreen(new ExamineBoard());
      break;
    case 3: 
      // continue game
      continueGame();
      break;
    default: break;    
  }
}

void GameMenu::continueCb(void * arg)
{
  GameMenu * self = (GameMenu*) arg;
  self->deselectItem();
  self->m_hilightItem = 3;
  self->selectItem();
  Misc::delay(TOUCH_PAUSE);
  self->a();
}

void GameMenu::examineCb(void * arg)
{
  GameMenu * self = (GameMenu*) arg;
  self->deselectItem();
  self->m_hilightItem = 2;
  self->selectItem();
  Misc::delay(TOUCH_PAUSE);
  self->a();
}

void GameMenu::selectSpellCb(void * arg)
{
  GameMenu * self = (GameMenu*) arg;
  int y = self->m_y - self->m_checking->area().y;
  int menuIndex = y/24;
  self->deselectItem();
  self->m_hilightItem = menuIndex;
  self->selectItem();
  Misc::delay(TOUCH_PAUSE);
  self->a();
}
