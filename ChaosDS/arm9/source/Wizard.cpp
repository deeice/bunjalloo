#include <algorithm>
#include <string.h>
#include <nds.h>
#include "ndspp.h"
#include "Wizard.h"
#include "Arena.h"
#include "Misc.h"
#include "Text16.h"
#include "ExamineCreature.h"
#include "WizardData.h"
#include "SpellData.h"

using namespace Misc;
static const int PLAYER_COUNT(8);
static const int WIZARD_COLOUR(1);
static const unsigned short s_defaultMap[] = {
  0, 1,
  2, 3
};

Wizard Wizard::s_players[PLAYER_COUNT];
const unsigned short int  s_chaosColours[] = {
  RGB5(31,31,31), // white
  RGB5(31,31,0), // yellow
  RGB5(0,31,31), //lightblue
  RGB5(0,31,0), //green
  RGB5(31,0,31), //purple
  RGB5(31,0,0), //red
  RGB5(0,0,31), //blue
  RGB5(20,20,20), //grey
  RGB5(20,20,0), // mustard
};

Wizard::Wizard()
{
  reset();
}

void Wizard::reset()
{
  m_combat = 0;
  m_rangedCombat = 0;
  m_range = 0;
  m_defence = 0;
  m_movementAllowance = 0;
  m_manoeuvreRating = 0;
  m_magicResistance = 0;
  m_spellCount = 0;
  m_ability = 0;
  m_image = 0;
  m_colour = 0;
  m_playerType = PLYR_HUMAN;
  m_modifierFlag = 0;
  m_illusionCast = 0;
  m_selectedSpell = 0;
  m_timid = 0;
  for (int i = 0; i < 40; ++i)
    m_spells[i] = 0;
}

void Wizard::resetPlayers()
{
  for (int i = 0; i < PLAYER_COUNT; ++i)
    s_players[i].reset();
}

static const char* const namesData[22] = {
  "Rich Q  ", "Fat Matt", "Colin   ", "Rich B  ", "Leek    ", "Dan     ",
  "Marce   ", "Smithy  ", "Kiff    ", "Ben     ", "Nick    ", "Tim     ",
  "Gandalf ", "H.Potter", "Houdini ", "Merlin  ", "Grotbags", "Venger  ",
  "Presto  ", "Saruman ", "Sooty   ", "Paul D  ", 
};

static inline bool array_contains(unsigned char * a, unsigned char n) {
  return std::find(a, a+PLAYER_COUNT, n) != (a+PLAYER_COUNT);
}

void Wizard::createDefaultWizards()
{
  // creates PLAYER_COUNT default wizards with sensible colours
  // names, type and colour are random and not repeated
  unsigned char names[PLAYER_COUNT] = {0};
  unsigned char colours[PLAYER_COUNT] = {0};
  unsigned char images[PLAYER_COUNT] = {0};
  
  for (int i = 0; i < PLAYER_COUNT; i++) {
    unsigned char tmp = 1+getRand(22);
    while (array_contains(names, tmp)) {
      tmp = 1+getRand(22);
    }
    names[i] = tmp;
    strcpy(s_players[i].m_name , namesData[names[i]-1]);
    trim(s_players[i].m_name);

    tmp = 1+getRand(PLAYER_COUNT);
    while (array_contains(images, tmp) ) {
      tmp = 1+getRand(PLAYER_COUNT);
    }
    images[i] = tmp;
    s_players[i].m_image = images[i]-1;
    
    tmp = 1+getRand(9);
    while (array_contains(colours, tmp) ) {
      tmp = 1+getRand(9);
    }
    colours[i] = tmp;
    s_players[i].m_colour = colours[i]-1; //chaos_cols[colours[i]-1];
    s_players[i].m_modifierFlag = 0;
    s_players[i].m_playerType = PLYR_CPU | ( 4 << 4);
    s_players[i].m_id = i;
  }
  
  s_players[0].m_playerType = PLYR_HUMAN;
}

const u8 s_positionTable[56] = {

  0x41,0x4d,0x00,0x00,0x00,0x00,0x00,0x00,
  0x17,0x81,0x8d,0x00,0x00,0x00,0x00,0x00,
  0x11,0x1d,0x81,0x8d,0x00,0x00,0x00,0x00,
  0x07,0x30,0x3e,0x93,0x9b,0x00,0x00,0x00,
  0x07,0x10,0x1e,0x80,0x97,0x8e,0x00,0x00,
  0x07,0x11,0x1d,0x60,0x6e,0x94,0x9a,0x00,
  0x00,0x07,0x0e,0x40,0x4e,0x90,0x97,0x9e
};
void Wizard::initialisePlayers()
{
  int playerCount = Arena::instance().players();
  u8 offset = (playerCount-2)*8;
  // in speccy chaos, the spell data for players starts at 7f47. 
  // For player 1, it goes 7f47 - 7f6e (inclusives) = 0x28 = 40 (20 spells)
  for (int loop = 0; loop < playerCount; loop++) {
    
    Wizard & player = s_players[loop];
    int cpuLevel = player.level();
    
    // generate the player stats
    player.m_combat = (getRand(10)/2) + 1 + (cpuLevel/2);
    player.m_rangedCombat = 0;
    player.m_range = 0;
    player.m_defence = (cpuLevel/2) + (getRand(10)/2) + 1;
    player.m_movementAllowance = 1;
    player.m_manoeuvreRating = (cpuLevel/4) + (getRand(10)/2) + 3;
    player.m_magicResistance = (getRand(10)/2) + 6;
    player.m_spellCount = cpuLevel + (getRand(10)/2) + 0xB;   
    if (player.m_spellCount > 20)
      player.m_spellCount = 20;
    
    player.m_ability = 0;
    if ( (5 - (cpuLevel/2)) <= (int)getRand(10))
      player.m_ability = getRand(10)/4;     
    
    // change from index representation to the actual 15 bit colour value
    // player.m_colour = s_chaosColours[player.m_colour];
     
    // generate the spells...
    int spellindex = 0;
    int spellid = 0x01;
    if (player.isCpu()) {
      // disbelieve has random priority
      player.m_spells[spellindex] = getRand(10)+0x0c;
    } else {
      player.m_spells[spellindex] = 0; // set to 0 for "no spell selected"
      player.m_selectedSpell = 0; // set to 0 for "no spell selected"
    }
    
    spellindex++;
    player.m_spells[spellindex] = spellid;
    spellindex++;

    for (int spell = 1; spell < player.m_spellCount; spell++) 
    {
      spellid = getRand(255) & 0x3F;
      // TODO : change this
      while (spellid < SPELL_KING_COBRA or spellid > SPELL_RAISE_DEAD) 
      {
        spellid = getRand(255) & 0x3F;
      }
       
      player.m_spells[spellindex] = s_spellData[spellid].castPriority;
      
      spellindex++;
      player.m_spells[spellindex] = spellid;
      spellindex++;
    }
#if 0
    spellindex++; 
    spellid = SPELL_MAGIC_WINGS;
    player.m_spells[spellindex] = spellid;
    player.m_spellCount++;
    spellindex++; 

    spellindex++; 
    spellid = SPELL_WRAITH;
    player.m_spells[spellindex] = spellid;
    player.m_spellCount++;
    spellindex++;
#endif
    player.m_spells[40] = 0;
    player.m_spells[41] = 0;
    if (player.isCpu()) {
      // order the spells by priority  
      orderTable(20, player.m_spells);
    }
    
    // set the player positions..
    int square = s_positionTable[offset+loop];
    Arena::instance().setWizardSquare(square, loop);
    
  }
  
  // FIXME: dead_wizards = 0;
}

int Wizard::getNextHuman(int startIndex) 
{
  for (int i = startIndex+1; i < Arena::instance().players(); ++i) 
  {
    if (not s_players[i].isCpu() and not s_players[i].isDead())
      return i;
  }
  return 9;
}

void Wizard::draw8(int x, int y, int frame)
{
  Arena::instance().setPalette8(x,y,9);
  u8 maskedGfx[128];
  
  u16 id;
  // 4 tiles per frame
  int index = frame * 4;
  
  // copy the actual graphics into the temp array
  // need to load it in blocks of 16 for 4bpp gfx
  for (int loop = 0; loop < 4; loop++) {
    // id = <the tile id in the map file at this position>
    id = s_wizardData[m_image].map[index];
    index++;
    swiCopy(&s_wizardData[m_image].gfx[id*16], &maskedGfx[loop*32], 16);
  }
  
  // replace the temp array index WIZARD_COLOUR (which is the colour that should be replaced)
  // with index playerid, which is the colour index for this player.
  u8 defaultCol_l = WIZARD_COLOUR;
  u8 defaultCol_h = WIZARD_COLOUR<<4;
  u8 newCol_l = m_id+8;
  u8 newCol_h = (m_id+8) << 4;
  for (int loop = 0; loop < 128; loop++) {
    u8 thisVal = maskedGfx[loop];
    
    // 4 bits per pixel...
    // mask the lower 4 bits
    if ((thisVal & 0x0f) == defaultCol_l) {
      thisVal &= ~defaultCol_l;
      thisVal |= newCol_l;
    }
    // mask the higher 4 bits
    if ((thisVal & 0xf0) == defaultCol_h) {
      thisVal &= ~defaultCol_h;
      thisVal |= newCol_h;
    }
    maskedGfx[loop] = thisVal;
  }
  Arena::instance().drawGfx8((unsigned short*)maskedGfx, s_defaultMap, x, y, 0);
}

void Wizard::draw(int x, int y, int frame)
{
  if ( hasShadowForm() and frame == 3) {
    // shadow form, do not draw this frame
    Arena::instance().clearSquare(x,y);
    return;
  }
  draw8(x*2, y*2, frame);
}

void Wizard::updateColour()
{
  // 9th palette
  nds::Palette p(0, 9);
  // 8+id is the colour index
  p[m_id+8] = getColour();
}

unsigned short Wizard::getColour()
{
  return s_chaosColours[m_colour];
}

Wizard * Wizard::getPlayers() {
  return s_players;
}

Wizard & Wizard::getCurrentPlayer() {
  return s_players[Arena::instance().currentPlayer()];
}

void Wizard::nameAt(int x, int y, int pal) const
{
  if (pal == -1)
    pal = m_id;
  Text16::instance().print(m_name, x, y, pal);
}

void Wizard::printLevelAt(int x, int y) const
{
  if ((m_playerType & PLYR_CPU)) {
    char str[30];
    strcpy(str, "Computer ");
    char istr[3];
    int level = this->level();
    Text16::int2a(level, istr);
    strcat(str, istr);
    Text16::instance().print(str, x, y, m_id);
  }
  else {
    Text16::instance().print("Human     ", x, y, m_id);
  }
}

int Wizard::level() const
{
  return m_playerType>>4;
}

void Wizard::level(int newLevel)
{
  if (not newLevel) {
    m_playerType = PLYR_HUMAN;
  }
  else {
    m_playerType = PLYR_CPU | (newLevel<<4);
  }
}

bool Wizard::isCpu() const
{
  return m_playerType & PLYR_CPU;
}
bool Wizard::isDead() const
{
  return (m_modifierFlag & 0x10);
}

int Wizard::colour() const
{
  return m_colour;
}
void Wizard::colour(int c) 
{
  m_colour = c;
}

int Wizard::image() const
{
  return m_image;
}
void Wizard::image(int i) 
{
  m_image = i;
}

void Wizard::removeNullSpells()
{
  if (isCpu()) return;

  int newCount = 0;
  u8 newSpells[20];
  for (int i = 0; i < 20; i++) {
    int index = i*2+1;
    if (m_spells[index] != 0) {
      newSpells[newCount++] = m_spells[index];
    }
  }
  // copy the valid spells back again
  for (int i = 0; i < 20; i++) {
    if (i < newCount) {
      m_spells[i*2]   = 0x12; // default priority...
      m_spells[i*2+1] = newSpells[i];
    } else {
      m_spells[i*2+1] = 0;
    }
  }
  m_spells[0] = 0;
  m_spellCount = newCount-1;
  
}

const char * const Wizard::name()
{
  return m_name;
}


const int Wizard::getSpellId(int index) const
{
  if (index > m_spellCount)
    return 0;
  return m_spells[1+index*2];
}
const SpellData * Wizard::getSpell(int index) const
{
  if (index > m_spellCount)
    return 0;
  return &s_spellData[getSpellId(index)];
}

int Wizard::spellCount() const
{
  return m_spellCount;
}

int Wizard::getAbility() const
{
  return m_ability;
}


void Wizard::displayData() const
{
  Text16 & text16(Text16::instance());

  // print name in yellow
  nameAt(2,1,1);
  bool need_comma(false);
  int x = 2;
  if (m_modifierFlag & 0x2) {
    // bit 1 set, "KNIFE"
    const char * const str = "KNIFE";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }
  if (m_modifierFlag & 0x4) {
    // bit 2 set,
    const char * const str = "SWORD";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }

  if ((m_modifierFlag & 0xC0) == 0xC0) {
    // "ARMOUR"
    const char * str = "ARMOUR";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }
  if ((m_modifierFlag & 0xC0) == 0x40) {
    // "SHIELD"
    const char * str = "SHIELD";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }

  if (hasMagicWings()) {
    // bit 5
    // FLYING
    const char * str = "FLYING";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }

  if (hasShadowForm()) {
    // bit 3
    // SHADOW
    const char * str = "SHADOW";
    if (need_comma) {
      // draw a comma...
      text16.print(",", x,3, 1);
      x++;
    }
    // write value
    text16.print(str, x,3, 1);
    x+=strlen(str);
    need_comma = true;
  }

  // now draw the actual stats... nice hack!
  ExamineCreature::drawStats(&this->m_combat);
  
  // spells and ability
  ExamineCreature::printStat(m_spellCount, 8, 2, 3);
  ExamineCreature::printStat(m_ability, 9, 2, 3);
}

void Wizard::removeSelectedSpell()
{
  m_spells[m_selectedSpell] = 0;
}

void Wizard::setNoSpell()
{
  m_selectedSpell = 0;
}

void Wizard::setSelectedSpell(int index)
{
  m_selectedSpell = index*2 + 1;
  m_illusionCast = false;
}

int Wizard::getSelectedSpellId() const
{
  if (m_selectedSpell)
    return m_spells[m_selectedSpell];
  else 
    return 0;
}

void Wizard::setIllusion(bool isIllusion)
{
  m_illusionCast = isIllusion;
}

void Wizard::updateCreatureCount() 
{
  
  if (Arena::instance().roundNumber() < 6) {
    m_timid = 0;
    return;
  }
  
  Arena & arena(Arena::instance());
  for (int i = 0; i < 0x9f; i++) {
    int creature = arena.get(0,i);
    if (creature >= SPELL_KING_COBRA and creature < SPELL_GOOEY_BLOB) 
    {
      // is a creature
      int underneath = arena.get(4,i);
      if (underneath != Arena::WIZARD_INDEX+m_id) {
        // is not a ridden creature
        int flags = arena.get(3,i);
        int owner = flags&0x7;
        if (owner == m_id) {
          m_timid = 0;
          return;
        }
      }
    }
  }
  
  // if we are past round 6 and have no creatures
  // then "be brave"...
  m_timid = 0x14;
}

void Wizard::doAISpell()
{
#if 0
  FIXME
  /*
    set spell 0 priorty = 0x0c + GetRand(10) (I imagine this is in case Disbelieve was top priority?)
    order the spell list by priority
    best spell = 0
    while (best spell < spell list length) {
      select the best spell 
      select the best square for this spell
      if no square is good
        best spell ++
      else
        break
    }
    
    if a spell has been chosen...
      cast spell at chosen square
      remove casted spell from list
    
    move on to next player...
  */
  
  if (IS_WIZARD_DEAD(players[current_player].modifier_flag)) {
    return;
  }
  
  temp_cast_amount = 0;
  players[current_player].spells[0] = 0x0c + GetRand(10);
  order_table(players[current_player].spell_count, players[current_player].spells);
  u8 best_spell = 0;
  while (best_spell < players[current_player].spell_count) {
    current_spell = players[current_player].spells[best_spell*2 + 1];
    // "cast" the spell... each casting routine has the CPU AI code for that spell built into it.
    // if no good square was found, then go to the next spell
    target_square_found = 0;
    if (current_spell != 0) {
      target_square_found = 1;
      set_current_spell_chance();
      CHAOS_SPELLS.pSpellDataTable[current_spell]->pFunc();

    }
    
    if (target_square_found) {
      
      // spell was cast succesfully
      if (current_spell != 0x1) {
        // spell used (if not disblv)
        players[current_player].spells[best_spell*2 + 1] = 0;
      }
//      break;
      return;
    } else {
      best_spell++;
    }
  }
  
#endif
}

void Wizard::setupHumanPlayerCast()
{
#if 0
  FIXME
  set_current_spell_chance();
  // print player name and spell...
  remove_cursor();
  print_name_spell();
  
  wait_for_keypress();
  redraw_cursor();
  // clear message display
  clear_message();
  
  temp_illusion_flag = players[current_player].illusion_cast;

  // NB: Spell Success is calculated twice for some spells!
  // not sure if that is a bug or not
  // e.g. for creatures, this code is called once -> here
  // Magic Wood, it is called here and again just before the cast
  set_spell_success();
  
#endif
}
