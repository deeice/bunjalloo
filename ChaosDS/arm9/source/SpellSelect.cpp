#include <string.h>
#include "ndspp.h"
#include "SpellSelect.h"
#include "Text16.h"
#include "Arena.h"
#include "Graphics.h"
#include "Wizard.h"

using namespace nds;
static const int ARROWHEAD_CHAR('+' - Text16::FIRST_CHAR_INDEX);

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
  //selectSpell(0);

  Video::instance().fade(false);
}

void SpellSelect::animate()
{}
CurrentScreen_t SpellSelect::getId() const
{
  return SCR_SELECT_SPELL;
}
void SpellSelect::handleKeys()
{}


void SpellSelect::initPalettes()
{
  // for each palette...
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
  while (loop < 8 && spellIndex < 20) 
  {
    //int index(1+spellIndex*2);
    if (currentPlayer.spell(spellIndex) != 0) {
      // TODO: spells should be object oriented
      // currentPlayer.spell(spellIndex)->printName(1,y);
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
