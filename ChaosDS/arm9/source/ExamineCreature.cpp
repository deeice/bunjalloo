#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "ndspp.h"
#include "ExamineCreature.h"
#include "GameState.h"
#include "Arena.h"
#include "Wizard.h"
#include "Text16.h"
#include "Graphics.h"
#include "SpellData.h"


// define the positions of the stats
typedef struct {
  u8 startx;
  u8 starty; 
  u8 statx;
} StatPosition_t;

const StatPosition_t s_statPosition[] = {
  {2, 5, 9},      // combat
  {2, 7, 16},     // ranged combat
  {19, 7, 25},    // range
  {2, 9, 10},     // defence
  {2, 11, 21},     // movement allowance
  {2, 13, 19},     // manoeuvre rating
  {2, 15, 19},     // magic resistance
  {2, 17, 17},     // casting chance
  {2, 17, 9},    // spells
  {12, 17, 20},     // ability
};
// 
static const char * const s_statStrings [] = {
  "COMBAT=",
  "RANGED COMBAT=",
  "RANGE=",
  "DEFENCE=",
  "MOVEMENT ALLOWANCE=",
  "MANOEUVRE RATING=",
  "MAGIC RESISTANCE=",
  "CASTING CHANCE=",      // 7
  "SPELLS=",              // 8
  "ABILITY=",             // 9
};

using namespace nds;

ExamineCreature::ExamineCreature(ScreenI * returnScreen):
  m_first(true), 
  m_showCastChance(false),
  m_returnScreen(returnScreen)
{
  Arena::instance().getCursorContents(m_creature, m_underneath, m_flags);
}
ExamineCreature::~ExamineCreature()
{
  // iprintf("~ExamineCreature;\n");
}

void ExamineCreature::show()
{
  Text16 & text16(Text16::instance());
  Arena & arena(Arena::instance());
  int creature(m_creature);
  int underneath(m_underneath);
  if (m_first) {
    text16.clear();
    arena.clear();
    Graphics::instance().clearPalettes();
    text16.setColour(1, Color(30,31,0));  // yel
    text16.setColour(2, Color(0,31,31));  // light blue
    text16.setColour(3, Color(30,31,31)); // white
    text16.setColour(4, Color(30,0,31));  // purple
    text16.setColour(5, Color(0,31,0));   // green
  } else {
    creature = m_underneath;
    underneath = 0;
  }

  // which creature is here?
  if (creature) {
    displayData(creature, underneath, m_flags);
  }
  Video::instance().fade(false);
}
void ExamineCreature::animate()
{}
CurrentScreen_t ExamineCreature::getId() const
{
  return SCR_EXAMINE_BOARD;
}
void ExamineCreature::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  if (keysSlow & KEY_B) {
    if (m_first && m_underneath) {
      Video::instance().fade();
      this->show();
    } else {
      Video::instance().fade();
      GameState::instance().nextScreen(m_returnScreen);
    }
  }
}

void ExamineCreature::printStat(int value, int index, int palette, 
    int palette2,
    bool percent)
{
  char statval[5];
  Text16::instance().print(
      s_statStrings[index], 
      s_statPosition[index].startx, 
      s_statPosition[index].starty, 
      palette);
  Text16::int2a(value, statval);
  if (percent)
    strcat(statval, "/");
  Text16::instance().print(
      statval, 
      s_statPosition[index].statx, 
      s_statPosition[index].starty, 
      palette2);

}

void ExamineCreature::drawStats(const unsigned char * stat_pointer) 
{
  // loop over the 7 stats... this is a nice way to do it as it works for creatures and wizards, 
  // but it relies on structures being unpadded (devkitadvance doesn't pad to 4 byte boundaries)
  for (int i = 0; i < 7; i++) {
    // write the description in lblue
    // write the stat in white
    printStat(*stat_pointer, i, 2, 3);
    // point to next stat
    stat_pointer++;
  }
}

// void display_creature_data(u8 id, u8 arena4, u8 arena3)
void ExamineCreature::displayData(int creature, int underneath, int flags)
{
  if (creature != 0) {
    // clear text screen...
    Text16 & text16(Text16::instance());
    text16.clear();
    Arena::instance().decorativeBorder(15, Color(0,31,0), 0);
    if (creature < Arena::WIZARD_INDEX) {
      // creature
      // c479
      // print the creature's name
      const SpellData & spellData(s_spellData[creature]);
      spellData.printName(2,1,1);

      // its chaos/law type
      if (spellData.chaosRating != 0) {
        int screen_x = 3+strlen(spellData.spellName);
        int col = 1;
        char str[30];
        if (spellData.chaosRating < 0) {
          // chaos value, drawn in purple
          col = 4; // purple
          text16.print("(CHAOS ", screen_x,1, col); 
          screen_x += 7;
          Text16::int2a((spellData.chaosRating*-1), str);
          text16.print(str, screen_x++,1, col); 
        } else {
          // law, drawn in light blue
          col = 2; // l blue
          text16.print("(LAW ", screen_x,1, col); 
          screen_x += 5;
          Text16::int2a(spellData.chaosRating, str);
          text16.print(str, screen_x++,1, col); 
        }
        text16.print(")", screen_x,1, col); 
      } // end chaos / law type display

      bool need_comma(false);
      int x(2);
      // creature's special skills
      if (creature >= SPELL_HORSE && creature <= SPELL_MANTICORE) {
        // mount...
        const char * str = "MOUNT";
        if (need_comma) {
          // draw a comma...
          text16.print(",", x,3, 5);
          x++;
        }
        // write value
        text16.print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }
      // draw any "inside" wizards - for mounts or trees/castles
      if (underneath >= Arena::WIZARD_INDEX) {
        text16.print("(", x,3, 5);
        x++;
        Wizard & player(Wizard::getPlayers()[underneath-Arena::WIZARD_INDEX]);
        player.nameAt(x,3,5);
        int namelength = strlen(player.name());
        if ((creature >= SPELL_PEGASUS && creature <= SPELL_GHOST) && (flags & 0x40)
            && namelength > 6) { 
          // undead flying mount - need to save space...
          x += 6;
        }
        else {
          x += namelength;
        }
        text16.print(")", x,3, 5);
        x++;
      }

      if (creature >= SPELL_PEGASUS && creature <= SPELL_GHOST) {
        // flying
        const char * str = "FLYING";
        if (need_comma) {
          // draw a comma...
          text16.print(",", x,3, 5);
          x++;
        }
        // write value
        text16.print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }

      if ( (flags & 0x40) ||  (creature >= SPELL_VAMPIRE && creature <= SPELL_ZOMBIE)){
        // undead...
        const char * str = "UNDEAD";
        if (need_comma) {
          // draw a comma...
          text16.print(",", x,3, 5);
          x++;
        }
        // write value
        text16.print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }

      drawStats(&spellData.combat);

      // draw casting chance too if needed...
      if (m_showCastChance) {
        int currentChance = spellData.getCastChance();
        printStat(currentChance*10, 7, 2, 2, true);
      }

    } else {
      // wizard
      // c419
      Wizard & player(Wizard::getPlayers()[creature-Arena::WIZARD_INDEX]);
      player.displayData();
    }
  }
}
