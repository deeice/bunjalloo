#include "libnds.h"
#include "ndspp.h"
#include "GameMenu.h"
#include "Text16.h"
#include "Arena.h"
#include "Casting.h"
#include "Graphics.h"
#include "Wizard.h"
#include "SpellSelect.h"
#include "SoundEffect.h"
#include "GameState.h"
#include "ExamineBoard.h"
#include "Rectangle.h"
#include "HotSpot.h"
#include "Misc.h"

using namespace nds;
static const int MENU_XPOS(4);
static const int MENU_YPOS(8);
static const int CHAOS_GAUGE_YPOS(4);
static const int MAX_GAME_MENU(3);

GameMenu::GameMenu()
{
  int width = 20*8;
  Rectangle selectSpellRect = {MENU_XPOS*8, MENU_YPOS*8, width, 16 };
  m_hotspots.push_back(new HotSpot(selectSpellRect, selectSpellCb, this));

  Rectangle examineRect = {MENU_XPOS*8, (3+MENU_YPOS)*8, width, 16 };
  m_hotspots.push_back(new HotSpot(examineRect, examineCb, this));

  Rectangle continueRect = {MENU_XPOS*8, (6+MENU_YPOS)*8, width, 16 };
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
  
  text16.print("1. SELECT SPELL", MENU_XPOS,MENU_YPOS, 0);  
  text16.print("2. EXAMINE BOARD", MENU_XPOS,MENU_YPOS+3, 1);  
  text16.print("3. CONTINUE WITH GAME", MENU_XPOS,MENU_YPOS+6, 2);
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
  switch (m_hilightItem) 
  {
    case 0: 
      // view spells
      Video::instance().fade();
      GameState::instance().setNextScreen(new SpellSelect());
      // show_spell_screen();
      break;
    case 1: 
      // view arena
      Video::instance().fade();
      GameState::instance().setNextScreen(new ExamineBoard());
      break;
    case 2: 
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
  self->m_hilightItem = 2;
  self->selectItem();
  Misc::delay(10);
  self->a();
}

void GameMenu::examineCb(void * arg)
{
  GameMenu * self = (GameMenu*) arg;
  self->deselectItem();
  self->m_hilightItem = 1;
  self->selectItem();
  Misc::delay(10);
  self->a();
}

void GameMenu::selectSpellCb(void * arg)
{
  GameMenu * self = (GameMenu*) arg;
  self->deselectItem();
  self->m_hilightItem = 0;
  self->selectItem();
  Misc::delay(10);
  self->a();
}
