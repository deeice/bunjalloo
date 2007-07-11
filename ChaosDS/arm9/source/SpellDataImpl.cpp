#include <string.h>
#include "Arena.h"
#include "Casting.h"
#include "Graphics.h"
#include "SpellData.h"
#include "Text16.h"
#include "Wizard.h"


void SpellData::printName(int x, int y, int pal) const
{
  char str[30];
  // set_current_spell_chance();
  int chance = realCastChance();
  chance++;
  fullText(str);
  Text16 & text16 = Text16::instance();
  if (pal == -1)
    pal = chance/2;
  text16.print("              ", x, y,  pal);
  text16.print(str, x, y,  pal);
}

// 92f9
int SpellData::realCastChance() const
{
  // sets the current spell chance based on default value, world chaos and wizard ability
  // just set to default for now
  int chance = castChance;
  int worldChaos = Casting::worldChaos();
  if (worldChaos > 0 and chaosRating > 0) {
    chance += (worldChaos)/4;
  } 
  else if (worldChaos < 0 and chaosRating < 0) {
    chance += (-worldChaos)/4;
  }
  // 9353 - add ability
  chance += Wizard::currentPlayer().ability();
  
  if (chance >= 10) {
    chance = 9;
  } 
  else if (chance < 0) {
    chance = 0;
  }
  return chance;
}

// generate_spell_string
void SpellData::fullText(char * str) const
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


int SpellData::mainColour(int frame) const
{
  if (not gfx or not map) {
    return 0;
  }
  if (frame > 3) {
    frame = 0;
  }
  
  // count the 16 most used colours
  u16 used[16] = {0};
  // use the frame to get the current colour..
  for (int m = frame*4; m < ((frame+1)*4); m++) { 
    int id = map[m];
    for (int i = 0; i < 16; i++) {
      // read 4 pixels from gfx array
      u16 pixel4 = gfx[id*16+i];
      used[pixel4&0xf]++;
      used[(pixel4>>4)&0xf]++;
      used[(pixel4>>8)&0xf]++;
      used[(pixel4>>12)&0xf]++;
    }
  }
  int maxUsed = 0;
  int maxIndex = 1;
  // don't count 0, as that is transparent and most likely max used
  for (int i = 1; i < 16; i++) {
    if (used[i] > maxUsed) {
      maxIndex = i;
      maxUsed = used[i];
    }
  }
  return maxIndex;
}

bool SpellData::isNewFeature() const
{
  return this->flags & NEW_FEATURE;
}

void SpellData::drawJusticeGfx(int frame) const
{
  Arena::instance().drawSilhouetteGfx(
      gfx, map, Graphics::s_chaosColours[frame] );
}
