#include <nds.h>
#include <algorithm>
#include <functional>
#include "ndspp.h"

#include "CreatePlayers.h"
#include "GameMenu.h"
#include "GameState.h"
#include "Arena.h"
#include "Graphics.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;
static const int PLAYER_WIZ_Y(2);

// from IF ScreenI
void CreatePlayers::show()
{
  Text16::instance().clear();
  Graphics::clearPalettes();
  Graphics::loadPalette(9,9);
  Arena::instance().setPlayers(2);
  
  m_hilightItem = 0;
  Graphics::instance().setAnimationParams(30,-8);
  
  m_hilightWizard = 0;
  
  Text16 & text16 = Text16::instance();
  text16.setColour(10, Color(31,30,30));
  text16.print("How many players?", 4, 1, 10);
  
  // create the default start wizards
  Wizard::createDefaultWizards();
  
  Arena::instance().reset();
  Arena::instance().decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 

  iprintf("Show\n");
  updatePlayers();
  
  selectItem(0);
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
CurrentScreen_t CreatePlayers::screenId() const
{
  return SCR_CREATE_PLAYERS;
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
}
// Other CreatePlayers functions ....
 
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
    DrawWizard(int highlightItem):m_index(0), 
                                  m_hilightItem(highlightItem), 
                                  m_text16(Text16::instance()) 
    { }
    
    // for each element in the player array...
    result_type operator() (argument_type & element) {
      element.updateColour();
      element.draw8(1, m_index*2 + PLAYER_WIZ_Y, 0);
      if (m_index == (m_hilightItem - 1)) {
        m_text16.setColour(m_index, Color(31,30,30));
      } 
      else {
        m_text16.setColour(m_index, Color(0,30,30));
      }
      element.printNameAt(6, 3+m_index*2);
      element.printLevelAt(16,3+m_index*2);
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
  Text16::instance().print(str, 22,1, 10);
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
      //PlaySoundFX(SND_MENU);
    }
  } else {
    Wizard & player = Wizard::player(m_hilightItem-1);
    if (player.isCpu())
    {
      int level = player.level();
      level--;
      player.setLevel(level);
      // PlaySoundFX(SND_MENU);
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
      // PlaySoundFX(SND_MENU);
    }
  } 
  else {
    Wizard & player = Wizard::player(m_hilightItem-1);
    int level = player.level();
    if (level < 8) {
      level++;
      player.setLevel(level);
      // PlaySoundFX(SND_MENU);
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
    //u16 keys = keysDown();
    //if ((keys & KEY_L) && (keys & KEY_R)) {
      Wizard::createDefaultWizards();
      Text16::instance().clear();
      Text16::instance().print("How many players?", 4,1, 10);
      Text16::instance().setColour(10, Color(0,30,30));
      updatePlayers();
    //}
  } else {
    // go to the edit name screen
    /*
    current_player = m_hilightItem -1;
    fade_down();
    show_editname();
    fade_up();
    */
  }
    
}

void CreatePlayers::start(void) {
  //PlaySoundFX(SND_CHOSEN);
  // was init_players 
  Wizard::initialisePlayers();
  Arena::instance().setCurrentPlayer(0);
  // round_count = 0; //FIXME - part of casting and movement
  // check if we need to show game menu...
  Wizard & player0 = Wizard::player(0);
  if (player0.isCpu()) 
    Arena::instance().setCurrentPlayer(Wizard::nextHuman());
  if (Arena::instance().currentPlayer() == 9) 
  {
    // there is no human player!
    // continue_game(); // FIXME
    // Arena::instance().continue();
  } else {
    Video::instance().fade();
    GameState::instance().setNextScreen(new GameMenu());
    // fade_up();
  }
}
