#include <algorithm>
#include <functional>
#include <string.h>
#include "libnds.h"
#include "ndspp.h"
#include "Wizard.h"
#include "Graphics.h"
#include "Arena.h"
#include "Misc.h"
#include "Casting.h"
#include "Text16.h"
#include "ExamineSquare.h"
#include "WizardData.h"
#include "WizardCPU.h"
#include "SpellData.h"
#include "SoundEffect.h"
#include "Options.h"

using namespace Misc;
using nds::Color;
static const int PLAYER_COUNT(8);
const int Wizard::WIZARD_COLOUR(1);
const unsigned short Wizard::DEFAULT_MAP[] = {
  0, 1,
  2, 3
};

Wizard Wizard::s_players[PLAYER_COUNT];
int Wizard::s_deadWizards(0);

int Wizard::deadWizards()
{
  return s_deadWizards;
}

void Wizard::resetDeadWizards()
{
  s_deadWizards = 0;
}
void Wizard::incrDeadWizards()
{
  s_deadWizards++;
}

Wizard::Wizard():m_computer(new WizardCPU(*this))
{
  reset();
}

Wizard::~Wizard()
{
  delete m_computer;
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
  std::for_each(s_players,
      s_players+PLAYER_COUNT, 
      std::mem_fun_ref(&Wizard::reset));
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
    unsigned char tmp = 1+rand(22);
    while (array_contains(names, tmp)) {
      tmp = 1+rand(22);
    }
    names[i] = tmp;
    strcpy(s_players[i].m_name , namesData[names[i]-1]);
    trim(s_players[i].m_name);

    tmp = 1+rand(PLAYER_COUNT);
    while (array_contains(images, tmp) ) {
      tmp = 1+rand(PLAYER_COUNT);
    }
    images[i] = tmp;
    s_players[i].m_image = images[i]-1;
    
    tmp = 1+rand(9);
    while (array_contains(colours, tmp) ) {
      tmp = 1+rand(9);
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
    player.m_combat = (rand(10)/2) + 1 + (cpuLevel/2);
    player.m_rangedCombat = 0;
    player.m_range = 0;
    player.m_defence = (cpuLevel/2) + (rand(10)/2) + 1;
    player.m_movementAllowance = 1;
    player.m_manoeuvreRating = (cpuLevel/4) + (rand(10)/2) + 3;
    player.m_magicResistance = (rand(10)/2) + 6;
    player.m_spellCount = cpuLevel + (rand(10)/2) + 0xB;   
    if (player.m_spellCount > 20)
      player.m_spellCount = 20;
    
    player.m_ability = 0;
    if ( (5 - (cpuLevel/2)) <= (int)rand(10))
      player.m_ability = rand(10)/4;     
    
    // change from index representation to the actual 15 bit colour value
    // player.m_colour = Graphics::s_chaosColours[player.m_colour];
     
    // generate the spells...
    int spellindex = 0;
    player.m_spells[spellindex] = 0; // set to 0 for "no spell selected"
    if (player.isCpu()) {
      // disbelieve has random priority
      player.m_spells[spellindex] = rand(10)+0x0c;
    } else {
      player.m_selectedSpell = 0; // set to 0 for "no spell selected"
    }
    
    int spellid = SPELL_DISBELIEVE;
    player.m_spells[spellindex+1] = spellid;
    spellindex += 2;

    int startIndex = 1;
    if (Options::instance().option(Options::NEW_FEATURES)) {
      spellid = SPELL_MEDITATE;
      player.m_spells[spellindex] = s_spellData[spellid].castPriority;
      player.m_spells[spellindex+1] = spellid;
      spellindex += 2;
      startIndex = 2;
    }

    for (int spell = startIndex; spell < player.m_spellCount; spell++) 
    {
      spellid = rand(255) & 0x3F;
      while (spellid < SPELL_KING_COBRA or spellid > SPELL_RAISE_DEAD) 
      {
        spellid = rand(255) & 0x3F;
      }
       
      player.m_spells[spellindex] = s_spellData[spellid].castPriority;
      
      spellindex++;
      player.m_spells[spellindex] = spellid;
      spellindex++;
    }
#ifdef CHEATING_DEBUG
    spellindex++; 
    spellid = SPELL_MAGIC_SLEEP;
    player.m_spells[spellindex] = spellid;
    player.m_spellCount++;
    spellindex++; 

    spellindex++; 
    spellid = SPELL_BLIND;
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

int Wizard::nextHuman(int startIndex) 
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
  Arena::instance().drawGfx8((unsigned short*)maskedGfx, DEFAULT_MAP, x, y, 0);
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
  p[m_id+8] = colourRGB();
}

unsigned short Wizard::colourRGB() const
{
  return Graphics::s_chaosColours[m_colour];
}

Wizard & Wizard::player(int index)
{
  return s_players[index];
}

Wizard * Wizard::players() {
  return s_players;
}

Wizard & Wizard::currentPlayer() {
  return s_players[Arena::instance().currentPlayer()];
}

void Wizard::printNameAt(int x, int y, int pal) const
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

void Wizard::setLevel(int newLevel)
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
void Wizard::setColour(int c) 
{
  m_colour = c;
}

int Wizard::image() const
{
  return m_image;
}
void Wizard::setImage(int i) 
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

const char * const Wizard::name() const
{
  return m_name;
}

void Wizard::setName(const char * name)
{
  strncpy(m_name, name, 12);
}


const int Wizard::spellId(int index) const
{
  if (index > m_spellCount)
    return 0;
  return m_spells[1+index*2];
}
const SpellData * Wizard::spell(int index) const
{
  if (index > m_spellCount)
    return 0;
  return &s_spellData[spellId(index)];
}

int Wizard::spellCount() const
{
  return m_spellCount;
}

int Wizard::ability() const
{
  return m_ability;
}


void Wizard::displayData() const
{
  Text16 & text16(Text16::instance());

  // print name in yellow
  printNameAt(2,1,1);
  bool need_comma(false);
  int x = 2;
  if (hasMagicKnife()) {
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
  if (hasMagicSword()) {
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

  if (hasMagicArmour()) {
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
  if (hasMagicShield()) {
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
  ExamineSquare::drawStats(&this->m_combat);
  
  // spells and ability
  ExamineSquare::printStat(m_spellCount, 8, 2, 3);
  ExamineSquare::printStat(m_ability, 9, 2, 3);
}

void Wizard::removeSelectedSpell()
{
  if (m_selectedSpell) {
    m_lastSpellCast = m_spells[m_selectedSpell];
  } else {
    m_lastSpellCast = 0;
  }
  // First spells are infinite.
  if (m_spells[m_selectedSpell] >= SPELL_KING_COBRA) {
    m_spells[m_selectedSpell] = 0;
  }
  m_selectedSpell = 0;
}

void Wizard::setSelectedSpell(int index)
{
  m_selectedSpell = index*2 + 1;
  m_illusionCast = false;
}

int Wizard::selectedSpellId() const
{
  if (m_selectedSpell)
    return m_spells[m_selectedSpell];
  else 
    return 0;
}

void Wizard::setIllusionCast(bool isIllusion)
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
  for (int i = 0; i < Arena::ARENA_SIZE; i++) {
    int creature = arena.at(0,i);
    if (creature >= SPELL_KING_COBRA and creature < SPELL_GOOEY_BLOB) 
    {
      // is a creature
      int underneath = arena.at(4,i);
      if (underneath != Arena::WIZARD_INDEX+m_id) {
        // is not a ridden creature
        int owner = arena.owner(i);
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

void Wizard::printNameSpell() const
{
  // print player name, the spell and the range - code from 967a
  Arena & arena(Arena::instance());
  if (not isCpu()) {
    int x,y;
    arena.currentPlayerXY(x,y);
    arena.setCursor(x, y);
  }
  arena.setBorderColour(m_id);
  Text16 & text16(Text16::instance());
  text16.clearMessage();

  // player name in yellow
  text16.setColour(12, Color(31,30,0));
  // spell in green
  text16.setColour(13, Color(0,30,0));

  int x = 0;
  printNameAt(x, Text16::MESSAGE_Y, 12);
  // do a sound effect...
  SoundEffect::play(SND_SPELLSTEP);
  Misc::delay(8);
  x += strlen(m_name) + 1;
  
  const SpellData & spellData(s_spellData[m_spells[m_selectedSpell]]);
  text16.print(spellData.spellName, x, Text16::MESSAGE_Y, 13);
  x += strlen(spellData.spellName) + 1;
  // do a sound effect...
  SoundEffect::play(SND_SPELLSTEP);
  Misc::delay(8);
  // range in white
  int rng = spellData.castRange / 2;
  if (rng >= 10) {
    rng = 20;
  }
  
  char str[3];
  Text16::int2a(rng, str);
  
  text16.print(str, x, Text16::MESSAGE_Y, 15);
  text16.setColour(15, Color(31,30,31));
  // do a sound effect...
  SoundEffect::play(SND_SPELLSTEP);
  Misc::delay(8);
}

void Wizard::waitForKeypress() 
{
  if (isCpu())
    return;
  bool pressed(false);
  while (not pressed) 
  {
    pressed = Misc::keypressWait();
  }
  
}


void Wizard::setupHumanPlayerCast()
{
  // set_current_spell_chance(); - not needed

  // print player name and spell...
  Arena::instance().enableCursor(false);
  printNameSpell();
  
  waitForKeypress();
  Arena::instance().enableCursor(true);

  // clear message display
  Text16::instance().clearMessage();
  
  // NB: Spell Success is calculated twice for some spells!
  // not sure if that is a bug or not
  // e.g. for creatures, this code is called once -> here
  // Magic Wood, it is called here and again just before the cast
  Casting::calculateSpellSuccess();
  
}



bool Wizard::isCastAllowed() const
{
  Text16 & text16(Text16::instance());
  Arena & arena(Arena::instance());
  int spellId = selectedSpellId();
  // check spell is in range .... call 9786
  if (not arena.isSpellInRange(spellId)) {
    text16.clearMessage();
    text16.displayMessage("OUT OF RANGE", Color(30,31,0));
    return false;
  }
  int creature, underneath, flags, frame;
  arena.cursorContents(creature, underneath, flags, frame);
  // in range, so do some more checks 
  // 9877...
  if (creature != 0) {
    if (spellId >= SPELL_MAGIC_WOOD) {
      return false;
    }
    if (not arena.isDead(arena.targetIndex())) {
      // creature is not dead, can't cast here.
      return false;
    }
  }
  // 9892... 
  // do tree check
  if (arena.isTreeAdjacent(spellId)) {
    return false;
  }
  // do wall check
  if (arena.isWallAdjacent(spellId, m_id)) {
    return false;
  }
  
  // do LOS check...
  if (arena.isBlockedLOS()) {
    text16.clearMessage();
    text16.displayMessage("NO LINE OF SIGHT", Color(0,30,31)); // lblue
    return false;
  }
  // call spell anim, etc...
  return true;
}

void Wizard::aiCast(int cast)
{ 
  m_computer->aiCast(cast); 
}

void Wizard::doAiSpell()
{
  m_computer->doAiSpell(); 
}

void Wizard::doAiMovement()
{
  m_computer->doAiMovement(); 
}

bool Wizard::hasTargetSquare() const
{
  return m_computer->hasTargetSquare();
}

void Wizard::setHasTargetSquare(bool has)
{
  m_computer->setHasTargetSquare(has);
}

void Wizard::kill()
{
  Arena & arena(Arena::instance());
  // code from b3c9
  arena.enableCursor(false);
  Misc::delay(4);
  incrDeadWizards();
  // do a sound fx..
  //int deadid = arena.atTarget() - Arena::WIZARD_INDEX;
  // convert the x, y target location to y, x tile locations
  // load the wizard pal into 10 too...
  Graphics::loadPalette(9,10);
  int x, y;
  arena.targetXY(x, y);
  arena.clearSquare(x-1, y-1);
  
  // FIXME: Looped sound
  SoundEffect::play(SND_SCREAM);
  
  int deadid = arena.atTarget()-Arena::WIZARD_INDEX;
  arena.wizardDeath(m_image);

  SoundEffect::play(SND_URGH);
  Graphics::clearPalettes();
  Graphics::loadAllPalettes();
  arena.setBorderColour(arena.currentPlayer());
  
  m_modifierFlag |= 0x10;
  
  Misc::delay(10);
  
  arena.destroyAllCreatures(deadid);
  if (not currentPlayer().isCpu())
    arena.enableCursor();
}

// set up the spell, do the cast the effect
void Wizard::setupRange0Spell()
{
  // code from 8357
  if (isCpu()) {
    printNameSpell();
    Misc::delay(80);
  }
  Casting::calculateSpellSuccess();
  Arena::instance().setCurrentPlayerIndex();
  Arena::instance().enableCursor(false);
  Casting::spellAnimation();
}


void Wizard::setHasMagicShield()
{
  m_image = GFX_MAGIC_SHIELD;
  m_modifierFlag &= 0x7f;
  m_modifierFlag |= 0x40;
}
void Wizard::setHasMagicArmour()
{
    m_image = GFX_MAGIC_ARMOUR;
    m_modifierFlag |= 0xC0;
}
void Wizard::setHasMagicSword()
{
    m_image = GFX_MAGIC_SWORD;
    m_modifierFlag &= 0xfc;      // 11111100  - low 2 bits set to 0
    m_modifierFlag |= 0x04;
}
void Wizard::setHasMagicKnife()
{
    m_image = GFX_MAGIC_KNIFE;
    m_modifierFlag &= 0xf8;      // 11111000  - low 3 bits set to 0
    m_modifierFlag |= 0x02;
}
void Wizard::setHasMagicBow()
{
    m_image = GFX_MAGIC_BOW;
    m_rangedCombat = 3;
    m_range = 6;
}
void Wizard::setHasMagicWings()
{
    m_image = GFX_MAGIC_WINGS;
    m_modifierFlag |= 0x20;
}

void Wizard::drawJusticeGfx(int frame) const
{
  Arena::instance().drawSilhouetteGfx(
      s_wizardData[m_image].gfx,
      s_wizardData[m_image].map,
      Graphics::s_chaosColours[frame] );
}

void Wizard::addSpell(int spellId)
{
  // find an empty slot for the spell
  int k = 3;
  for (int j = 0; j < 0x13; j++) {
    if (m_spells[k] == 0) {
      m_spells[k] = spellId;
      m_spellCount++;
      break;
    }
    k+=2;
  }
}

void Wizard::orderSpells()
{
  m_spells[0] = 12 + Misc::rand(10);
  Misc::orderTable(m_spellCount, m_spells);
}

void Wizard::newRandomSpell()
{
  char str[30];
  strcpy(str,"NEW SPELL FOR ");
  strcat(str,m_name);
  Text16::instance().displayMessage(str, Color(30,30,0)); // yellow
  Misc::delay(100);

  // generate new spell...
  Text16::instance().clearMessage();
  u8 randspell = Misc::rand(127);
  while (randspell < SPELL_KING_COBRA or randspell > SPELL_TURMOIL)
  {
    randspell = Misc::rand(127);
  }
  addSpell(randspell);
}
