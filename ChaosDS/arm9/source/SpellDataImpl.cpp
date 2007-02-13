#include <string.h>
#include <stdio.h>
#include "SpellData.h"
#include "Text16.h"

void SpellData::printName(int x, int y) const
{
  char str[30];
  // set_current_spell_chance();
  getFullText(str);
  // current_spell_chance++;
  // FIXME!
  Text16 & text16 = Text16::instance();
  //text16.print("              ", x, y,  current_spell_chance/2);
  //text16.print(str, x, y,  current_spell_chance/2);
  text16.print("              ", x, y,  castChance/2);
  text16.print(str, x, y,  castChance/2);
  iprintf("%s\n",str);
}

// generate_spell_string
void SpellData::getFullText(char * str) const
{
  char chaoslvl = '-';
  if (this->chaosRating > 0)
    chaoslvl = '+';
  else if (this->chaosRating < 0)
    chaoslvl = '*';
  str[0] = chaoslvl;
  str[1] = 0;
  strcat(str,this->spellName);
}
