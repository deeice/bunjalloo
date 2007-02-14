#include <nds.h>
#include <string.h>
#include "ndspp.h"
#include "SpellData.h"
#include "SpellSelect.h"
#include "Text16.h"
#include "Arena.h"
#include "Graphics.h"
#include "Wizard.h"
#include "GameState.h"
#include "GameMenu.h"

using namespace nds;
static const int ARROWHEAD_CHAR('+' - Text16::FIRST_CHAR_INDEX);
const static u16 s_castingChancePalette[6] = {
  Color(31,0,0),    // Red = 0-10%
  Color(31,0,31),   // Purple = 20-30%   
  Color(0,31,0),    // Green = 40-50%    
  Color(0,31,31),   // LightBlue = 60-70%
  Color(31,31,0),   // Yellow = 80-90%   
  Color(31,31,31),  // White = 100%      
};

// implement ScreenI
void SpellSelect::show()
{
  char str[30];
  
  m_topIndex = 0;
  m_hilightItem = 0;
  
  Wizard & currentPlayer(Wizard::getCurrentPlayer());
  // need to sort out the players spells here...
  // should remove all 0 spells and update spell count accordingly
  currentPlayer.removeNullSpells();
  Text16 & text16(Text16::instance());
  Text16::instance().clear();
  Arena::instance().clear();
  Graphics::instance().clearPalettes();
  // set up the palette so that 0-5 are the spell colours
  initPalettes();
  
  // 6 can be the default colour, 10 is the selected colour
  strcpy(str, currentPlayer.name());
  strcat(str, "'S SPELLS");
  text16.print(str, 1,1, 6);  
  text16.setColour(6, Color(0,30,30));
  
  // ask if they want an illusion and wait for response
  text16.setColour(14, Color(21,21,29));
  text16.print("PRESS R TO", 18,4, 14);  
  text16.print("EXAMINE", 18,6, 14);
  
  // write all the spells
  listSpells();
  selectSpell();

  Video::instance().fade(false);
}

void SpellSelect::animate()
{
  int spellIndex = m_hilightItem+m_topIndex;
  const SpellData * currentSpell = Wizard::getCurrentPlayer().getSpell(spellIndex);
  int chance = currentSpell->getCastChance()+1;
  
  Color c(s_castingChancePalette[chance/2]);
  Graphics::instance().animateSelection(10, c);
}

CurrentScreen_t SpellSelect::getId() const
{
  return SCR_SELECT_SPELL;
}

void SpellSelect::handleKeys()
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
  if (keysSlow & KEY_A) {
    a();
  } 
  if (keysSlow & KEY_B) {
    b();
  } 
}
// end of ScreenI implementation

void SpellSelect::initPalettes()
{
  // for each palette...
  for (int loop = 0; loop < 6; loop++) {
    Text16::instance().setColour(loop, s_castingChancePalette[loop]);
  }
}

void SpellSelect::listSpells() {
  // list the spells for current_player
  // this needed a rethink because 20 spells can't fit on screen
  // a list of 7 spells, "scroll" the list when you reach the top or bottom
  //  - left and right jump a page of spells up/down
  
  // need to know... current top index, spell count, selected index.
  // when press down, move down one, but don't scroll unless selected - top_index == 6 and we aren't at the end of the lsit
  // when up is pressed, move up one, but don't scroll unless selected - top_index == 1
  // also, shows arrows to show more spells are available off the end of the list
  
  int loop = 0;
  // set the first spell index to the spell at the top of the list
  int spellIndex(m_topIndex);
  int y(3);
  Wizard & currentPlayer(Wizard::getCurrentPlayer());
  while (loop < 8 and spellIndex < 20) 
  {
    //int index(1+spellIndex*2);
    const SpellData * spell = currentPlayer.getSpell(spellIndex);
    if (spell != 0) {
      spell->printName(1,y);
      // print_spell_name(players[current_player].spells[index], 1, y);
      y+=2;
      loop++;
    }
    spellIndex++;
  }
  
  int x;
  // draw the up down arrows...
  // these are the first part of character "^" flipped vertically for "v"
  // a bit of a hack - it uses the top half of the law symbol!
  
  x = 16; y = 3;
  Text16::instance().setColour(7, Color(31,0,0));

  unsigned char letter(ARROWHEAD_CHAR);
  if (m_topIndex <= 0) {
    letter = 0;
  }
  Text16::instance().putChar(letter, x, y, 7, 0);
  
  y = 17;
  
  letter = ARROWHEAD_CHAR;
  if ((m_topIndex + 7) >= currentPlayer.spellCount()) {
    letter = 0;
  }
  Text16::instance().putChar(letter, x, y, 7, TILE_FLIP_VERT);
}


void SpellSelect::up(void) {
  // need to redo the spell list when moving up, possibly.
  // to move up, deselect the current spell
  // change the m_hilightItem
  // select the new hilite item
  // BUT if when you move up top_index > 0
  // check if you need to scroll the list a bit
  
  if (m_topIndex == 0) {
    // no need to scroll
    deselectSpell();
    if (m_hilightItem > 0)
      m_hilightItem--;
    selectSpell();
  } else {
    deselectSpell(m_hilightItem);
    if (m_hilightItem > 1)
      m_hilightItem--;
    else {
      m_topIndex--;
      listSpells();
    }    
    selectSpell();
  }
  
    
}

void SpellSelect::down(void) {
  int spellCount = Wizard::getCurrentPlayer().spellCount();
  if ( (m_topIndex + 7) >= spellCount) {
    // no need to scroll
    deselectSpell();
    if ((m_hilightItem + m_topIndex) < spellCount)
      m_hilightItem++;
    selectSpell();
  } else {
    deselectSpell();
    if (m_hilightItem < 6)
      m_hilightItem++;
    else {
      m_topIndex++;
      listSpells();
    }    
    selectSpell();
  }
  
}

void SpellSelect::left(void) {
  // go to top of list
  deselectSpell();

  m_topIndex = 0;
  m_hilightItem = 0;
  
  listSpells();
  selectSpell();
}
void SpellSelect::right(void) {
  // go to end of list
  deselectSpell();
  int spellCount = Wizard::getCurrentPlayer().spellCount();
  if (  spellCount < 8) {
    m_hilightItem = spellCount-1;
  } else {
    m_topIndex = spellCount - 8;
    m_hilightItem = 7;
  }
  listSpells();
  selectSpell();
}

void SpellSelect::r(void) {
  
#if 0
  // cast_chance_needed is for examine_spell, whether to show the chance or not
  cast_chance_needed = 1;
  u8 spellindex = m_hilightItem+m_topIndex;
  u8 selected_spell = players[current_player].spells[spellindex*2+1];
  arena[0][15] = selected_spell;
  
  u8 ti = m_topIndex;
  u8 hi = m_hilightItem;
  examine_spell(15);
  arena[0][15] = 0;
  fade_down();
  show_spell_screen();
  cast_chance_needed = 0;
  m_hilightItem = hi;
  m_topIndex = ti;
  listSpells();
  select_spell(hi);
  fade_up();
#endif
}


void SpellSelect::a(void) {
  
#if 0
  // store the spell...
  int spellindex = m_hilightItem+m_topIndex;
  players[current_player].illusion_cast = 0;
  players[current_player].selected_spell = spellindex*2+1;
  // check for illusion...
  if (players[current_player].spells[players[current_player].selected_spell] < SPELL_KING_COBRA ||
    players[current_player].spells[players[current_player].selected_spell] >= SPELL_GOOEY_BLOB) {
    PlaySoundFX(SND_CHOSEN);
    fade_down();
    show_game_menu();
    fade_up();
    return;
  }

  // ask if they want an illusion and wait for response
  set_text16_colour(14, 0);  
  set_text16_colour(12, RGB16(31,31,31)); // white
  print_text16("ILLUSION?", 18,8, 12);  
  print_text16("(A=YES B=NO)", 17,10, 12);

  wait_for_letgo();
  while (1) {
    wait_vsync_int();
    anim_spell_select();
    UpdateKeys();
    if (KeyPressedNoBounce(KEY_A,50))
      break;
    if (KeyPressedNoBounce(KEY_B,50)) {
      PlaySoundFX(SND_CHOSEN);
      wait_for_letgo();
      fade_down();
      show_game_menu();
      fade_up();
      return;
    }
  }
  PlaySoundFX(SND_CHOSEN);
  wait_for_letgo();
  players[current_player].illusion_cast = 1;
  fade_down();
  show_game_menu();
  fade_up();
  
#endif
}

void SpellSelect::b(void) {
  Video::instance().fade();
  GameState::instance().nextScreen(new GameMenu());
}


void SpellSelect::selectSpell() {
  // select the spell at index "item"
  
  // get the actual spell id index
  int spellIndex = m_hilightItem+m_topIndex;
  const SpellData * currentSpell = Wizard::getCurrentPlayer().getSpell(spellIndex);
  
  currentSpell->printName(1,3+m_hilightItem*2, 10);
  
  int chance = currentSpell->getCastChance() + 1;
  Text16 & text16 = Text16::instance();
  text16.print("CASTING", 18,14, 2);
  text16.print("CHANCE=    ", 18,16, 2);
  char str[30];
  Text16::int2a(chance*10, str);
  strcat(str,"/");
  text16.print(str, 25,16, 5);
  
  Color colour(s_castingChancePalette[chance/2]);
  colour.red(colour.red()+31);
  colour.green(colour.green()+31);
  colour.blue(colour.blue()+31);
  
  text16.setColour(10, colour);
  Graphics::instance().setAnimationParams(-15,-8);
}

void SpellSelect::deselectSpell(int item) {
  
  // get the actual spell id index
  int spellIndex = item+m_topIndex;
  const SpellData * spell = Wizard::getCurrentPlayer().getSpell(spellIndex);
  if (spell)
    spell->printName(1,3+(item*2));
}
