#include "libnds.h"
#include "ndspp.h"
#include <string.h>
#include "Arena.h"
#include "ExamineSquare.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "SpellData.h"
#include "Text16.h"
#include "Wizard.h"


//! define the positions of the stats
typedef struct {
  u8 startx; //!< x position of stat description.
  u8 starty; //!< y position of stat.
  u8 statx;  //!< x position of stat numeric value.
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

ExamineSquare::ExamineSquare()
{
  m_shown = false;
  initialise();
}

void ExamineSquare::initialise()
{
  m_first = true;
  m_showCastChance = false;
  m_counter = 0;
  Arena::instance().cursorContents(m_creature, m_underneath, m_flags);
  m_index = Arena::instance().targetIndex();
}

void ExamineSquare::show()
{
  m_text = &Text16::instance();
  int creature(m_creature);
  int underneath(m_underneath);
  if (m_first) {
    m_text->clear();
    //arena.clear();
    //Graphics::instance().clearPalettes();
    m_text->setColour(1, Color(30,31,0));  // yel
    m_text->setColour(2, Color(0,31,31));  // light blue
    m_text->setColour(3, Color(30,31,31)); // white
    m_text->setColour(4, Color(30,0,31));  // purple
    m_text->setColour(5, Color(0,31,0));   // green
  } else {
    creature = m_underneath;
    underneath = 0;
  }

  m_text->clear();
  // which creature is here?
  if ((creature >= SPELL_KING_COBRA and creature < SPELL_GOOEY_BLOB)
      or (creature >= Arena::WIZARD_INDEX and not m_showCastChance))
  {
    // it is a creature or a wizard (and we want to see wizards)
    displayCreatureData(creature, underneath, m_flags);
  } else {
    // it is another type of spell
    displaySpellData(creature);
  }
  Misc::delay(1, false);
  Video::instance(1).setFade(0);
  m_shown = true;
}

void ExamineSquare::animate()
{
  if (m_shown) {
    m_counter++;
    if (m_counter == 50)
    {
      next();
    }
  }
}

void ExamineSquare::handleKeys()
{
  next();
}

void ExamineSquare::exitCb(void * arg)
{
  ((ExamineSquare*)arg)->next();
}

int ExamineSquare::index() const
{
  return m_index;
}

void ExamineSquare::next()
{
  m_counter = 0;
  Text16::drawToTop();
  if (m_first and m_underneath) {
    m_first = false;
    this->show();
  } 
  else if (m_underneath)
  {
    m_first = true;
    this->show();
  }
  Text16::drawToBottom();
}

void ExamineSquare::printStat(int value, int index, int palette, 
    int palette2,
    bool percent)
{
  char statval[5];
  Text16::drawToTop();
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
  Text16::drawToBottom();

}

void ExamineSquare::drawStats(const unsigned char * stat_pointer) 
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
void ExamineSquare::displayCreatureData(int creature, int underneath, int flags)
{
  if (creature != 0) {
    Arena::instance().decorativeBorder(15, Color(0,31,0), 0, Arena::HEIGHT, Arena::ALT_SCREEN);
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
          m_text->print("(CHAOS ", screen_x,1, col); 
          screen_x += 7;
          Text16::int2a((spellData.chaosRating*-1), str);
          m_text->print(str, screen_x++,1, col); 
        } else {
          // law, drawn in light blue
          col = 2; // l blue
          m_text->print("(LAW ", screen_x,1, col); 
          screen_x += 5;
          Text16::int2a(spellData.chaosRating, str);
          m_text->print(str, screen_x++,1, col); 
        }
        m_text->print(")", screen_x,1, col); 
      } // end chaos / law type display

      bool need_comma(false);
      int x(2);
      // creature's special skills
      if (creature >= SPELL_HORSE and creature <= SPELL_MANTICORE) {
        // mount...
        const char * str = "MOUNT";
        if (need_comma) {
          // draw a comma...
          m_text->print(",", x,3, 5);
          x++;
        }
        // write value
        m_text->print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }
      // draw any "inside" wizards - for mounts or trees/castles
      if (underneath >= Arena::WIZARD_INDEX) {
        m_text->print("(", x,3, 5);
        x++;
        Wizard & player(Wizard::player(underneath-Arena::WIZARD_INDEX));
        player.printNameAt(x,3,5);
        int namelength = strlen(player.name());
        if ( (creature >= SPELL_PEGASUS and creature <= SPELL_GHOST) 
            and (flags & 0x40) 
            and namelength > 6) 
        { 
          // undead flying mount - need to save space...
          x += 6;
        }
        else {
          x += namelength;
        }
        m_text->print(")", x,3, 5);
        x++;
      }

      if (creature >= SPELL_PEGASUS and creature <= SPELL_GHOST) {
        // flying
        const char * str = "FLYING";
        if (need_comma) {
          // draw a comma...
          m_text->print(",", x,3, 5);
          x++;
        }
        // write value
        m_text->print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }

      if ( (flags & 0x40) or  (creature >= SPELL_VAMPIRE and creature <= SPELL_ZOMBIE))
      {
        // undead...
        const char * str = "UNDEAD";
        if (need_comma) {
          // draw a comma...
          m_text->print(",", x,3, 5);
          x++;
        }
        // write value
        m_text->print(str, x,3, 5);
        x += strlen(str);
        need_comma = true;
      }

      drawStats(&spellData.combat);

      // draw casting chance too if needed...
      if (m_showCastChance) {
        printStat(spellData.realCastChance()*10, 7, 2, 2, true);
      }

    } else {
      // wizard
      // c419
      Wizard & player(Wizard::player(creature-Arena::WIZARD_INDEX));
      player.displayData();
    }
  }
}

// void display_spell_data(u8 id);
void ExamineSquare::displaySpellData(int spellId)
{
  // code from 94e2
  // this is used for displaying a spell sheet...
  // set up the colours
  Arena::instance().decorativeBorder(15, Color(0,0,31), Color(0,31,31), Arena::HEIGHT, Arena::ALT_SCREEN); 
  const SpellData & spellData(s_spellData[spellId]);
  // write spell name
  int start_x = 5;
  int start_y = 4;
  spellData.printName(start_x,start_y,1);
  
  start_y += 2;
  // its chaos/law type
  if (spellData.chaosRating != 0) {
    int screen_x = start_x;
    int col = 1;
    char str[30];
    if (spellData.chaosRating < 0) {
      // chaos value, drawn in purple
      col = 4; // purp
      m_text->print("(CHAOS ", screen_x,start_y, col); 
      screen_x += 7;
      
      Text16::int2a((spellData.chaosRating*-1), str);
      m_text->print(str, screen_x++,start_y, col); 
      
    } else {
      // law, drawn in light blue
      col = 2; // l blue
      m_text->print("(LAW ", screen_x,start_y, col); 
      screen_x += 5;
      
      Text16::int2a(spellData.chaosRating, str);
      m_text->print(str, screen_x++,start_y, col); 

      
    }
    m_text->print(")", screen_x,start_y, col); 
    
  } // end chaos / law type display
  start_y += 4;
  // casting chance...
  m_text->print(s_statStrings[7], start_x,start_y, 5);
  char statval[10];
  Text16::int2a(spellData.realCastChance()*10, statval);
  strcat(statval, "/");
  
  m_text->print(statval, start_x+15, start_y, 1);
  start_y+=4;
  // casting range
  m_text->print(s_statStrings[2], start_x,start_y, 5);
  u8 A = spellData.castRange>>1;
  if (A > 10)
    A = 20;
  Text16::int2a(A, statval);
  
  m_text->print(statval, start_x+6, start_y, 1);
}

void ExamineSquare::showCastChance(bool castChance) {
  m_showCastChance = castChance;
}

void ExamineSquare::hide()
{
  if (m_shown) {
    Misc::delay(1, false);
    Video::instance(1).setFade(16);
    m_shown = false;
  }
}

bool ExamineSquare::isHidden() const
{
  return not m_shown;
}
