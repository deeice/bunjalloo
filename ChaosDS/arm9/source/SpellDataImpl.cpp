#include <string.h>
#include <stdio.h>
#include "SpellData.h"
#include "Casting.h"
#include "Text16.h"
#include "Wizard.h"

void SpellData::printName(int x, int y, int pal) const
{
  char str[30];
  // set_current_spell_chance();
  int chance = getCastChance();
  chance++;
  getFullText(str);
  Text16 & text16 = Text16::instance();
  if (pal == -1)
    pal = chance/2;
  text16.print("              ", x, y,  pal);
  text16.print(str, x, y,  pal);
}

// 92f9
int SpellData::getCastChance() const
{
  // sets the current spell chance based on default value, world chaos and wizard ability
  // just set to default for now
  int chance = castChance;
  int worldChaos = Casting::getWorldChaos();
  if (worldChaos > 0 and chaosRating > 0) {
    chance += (worldChaos)/4;
  } 
  else if (worldChaos < 0 and chaosRating < 0) {
    chance += (-worldChaos)/4;
  }
  // 9353 - add ability
  chance += Wizard::getCurrentPlayer().getAbility();
  
  if (chance >= 10) {
    chance = 9;
  } 
  else if (chance < 0) {
    chance = 0;
  }
  return chance;
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
