#include "libnds.h"
#include <string.h>
#include "ndspp.h"
#include "Arena.h"
#include "ExamineSquare.h"
#include "GameMenu.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "IllusionPicker.h"
#include "Options.h"
#include "SoundEffect.h"
#include "SpellData.h"
#include "SpellSelect.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;
static const int ARROWHEAD_CHAR('+' - Text16::FIRST_CHAR_INDEX);
static const int SPELL_NAME_X(1+1);
static const int SPELL_NAME_WIDTH(14);
static const int SPELL_START_INDEX(8);
static const int SPELL_END_INDEX(20);
static const int ARROW_X(16+1);
static const int ARROW_UP_Y(3+1);
static const int ARROW_DOWN_Y(17+1);
static const int RETURN_MENU_X(12);
static const int RETURN_MENU_Y(21);

static const int CAST_CHANCE_POS_X(19+1);

static const int PLAYER_NAME_X(1+1);
static const int PLAYER_NAME_Y(1+1);

static const int CAST_CHANCE_POS_Y(14);

const static u16 s_castingChancePalette[6] = {
  Color(31,0,0),    // Red = 0-10%
  Color(31,0,31),   // Purple = 20-30%   
  Color(0,31,0),    // Green = 40-50%    
  Color(0,31,31),   // LightBlue = 60-70%
  Color(31,31,0),   // Yellow = 80-90%   
  Color(31,31,31),  // White = 100%      
};

SpellSelect::SpellSelect(bool examine):
  m_hilightItem(0), m_topIndex(0) , m_examine(examine), m_examineScreen(0)
{ 
  // register callbacks for touch screen
  // "scrollbar" - the 2 little arrows:
  Rectangle upRect = {(ARROW_X*8)-4, ARROW_UP_Y*8, 16, 16};
  m_hotspots.push_back(new HotSpot(upRect, scrollUpCb, this));
  Rectangle downRect = {(ARROW_X*8)-4, ARROW_DOWN_Y*8, 16, 16};
  m_hotspots.push_back(new HotSpot(downRect, scrollDownCb, this));

  // spell selection - one big rectangle
  Rectangle spellRect = {
    SPELL_NAME_X*8, 
    ARROW_UP_Y*8, 
    SPELL_NAME_WIDTH*8, 
    8*16
  };
  m_hotspots.push_back(new HotSpot(spellRect, spellSelectCb, this));

  // return to menu
  Rectangle returnRect = {RETURN_MENU_X*8, RETURN_MENU_Y*8, 14*8, 16};
  m_hotspots.push_back(new HotSpot(returnRect, returnCb, this));
}

SpellSelect::~SpellSelect()
{
  delete m_examineScreen;
}

// implement ScreenI
void SpellSelect::show()
{
  char str[30];
  
  Wizard & currentPlayer(Wizard::currentPlayer());
  // need to sort out the players spells here...
  // should remove all 0 spells and update spell count accordingly
  currentPlayer.removeNullSpells();
  Text16 & text16(Text16::instance());
  Arena & arena(Arena::instance());
  text16.clear();
  arena.clear();
  // remove any examined spell...
  arena.setSpellAt(15, 0, 0);
  Graphics::instance().clearPalettes();
  // set up the palette so that 0-5 are the spell colours
  initPalettes();
  
  // 6 can be the default colour, 10 is the selected colour
  strcpy(str, currentPlayer.name());
  strcat(str, "'S SPELLS");
  text16.print(str, PLAYER_NAME_X, PLAYER_NAME_Y, 6);  
  text16.setColour(6, Color(0,30,30));
  
  text16.setColour(14, Color(21,21,29));
  text16.print("RETURN TO MENU", RETURN_MENU_X, RETURN_MENU_Y, 14);
  
  // write all the spells
  listSpells();
  selectSpell();

  arena.decorativeBorder(15, Color(31,31,0), Color(0,31,0));
  Video::instance().fade(false);
}

void SpellSelect::animate()
{
  int spellIndex = m_hilightItem+m_topIndex;
  const SpellData * currentSpell = Wizard::currentPlayer().spell(spellIndex);
  int chance = currentSpell->realCastChance()+1;
  
  Color c(s_castingChancePalette[chance/2]);
  Graphics::instance().animateSelection(10, c);
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
  if (keysSlow & KEY_R) {
    r();
  } 
  if (keysSlow & KEY_A) {
    a();
  } 
  if (keysSlow & KEY_B) {
    b();
  } 
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  } 
}
// end of ScreenI implementation
//

void SpellSelect::scrollUpCb(void * arg)
{
  SpellSelect * self = (SpellSelect*)arg;
  if (self->m_topIndex > 0) {
    // scroll up one row
    self->scrollUp();
  }
}

void SpellSelect::scrollDownCb(void * arg)
{
  SpellSelect * self = (SpellSelect*)arg;
  self->scrollDown();
}
void SpellSelect::spellSelectCb(void * arg)
{
  SpellSelect * self = (SpellSelect*)arg;
  int y = self->m_y - self->m_checking->area().y;
  int spellIndex = y/16;
  int spellCount = Wizard::currentPlayer().spellCount();
  if (spellIndex != self->m_hilightItem) {
    if ( spellIndex <= spellCount ) {
      self->deselectSpell();
      self->m_hilightItem = spellIndex;
      self->selectSpell();
    }
    if (Options::instance().option(Options::ONE_TOUCH))
    {
      self->a();
    }
  }
  else {
    self->a();
  }
}

void SpellSelect::returnCb(void * arg)
{
  SpellSelect * self = (SpellSelect*)arg;
  self->b();
}

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
  int y(ARROW_UP_Y);
  Wizard & currentPlayer(Wizard::currentPlayer());
  while (loop < SPELL_START_INDEX and spellIndex < SPELL_END_INDEX) 
  {
    //int index(1+spellIndex*2);
    const SpellData * spell = currentPlayer.spell(spellIndex);
    if (spell != 0) {
      spell->printName(SPELL_NAME_X,y);
      y+=2;
      loop++;
    }
    spellIndex++;
  }
  
  // draw the up down arrows...
  // these are the first part of character "^" flipped vertically for "v"
  // a bit of a hack - it uses the top half of the law symbol!
  
  int x = ARROW_X; 
  y = ARROW_UP_Y;
  Text16::instance().setColour(7, Color(31,0,0));

  unsigned char letter(ARROWHEAD_CHAR);
  if (m_topIndex <= 0) {
    letter = 0;
  }
  Text16::instance().putChar(letter, x, y, 7, 0);
  
  y = ARROW_DOWN_Y;
  
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

void SpellSelect::down() {
  int spellCount = Wizard::currentPlayer().spellCount();
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

void SpellSelect::scrollUp()
{
  deselectSpell();
  m_topIndex--;
  if (m_hilightItem < 7)
    m_hilightItem++;
  listSpells();
  selectSpell();
}

void SpellSelect::scrollDown()
{
  int spellCount = Wizard::currentPlayer().spellCount();
  if ( (m_topIndex + 7) < spellCount) {
    deselectSpell();
    m_topIndex++;
    if (m_hilightItem > 0)
      m_hilightItem--;
    listSpells();
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
  int spellCount = Wizard::currentPlayer().spellCount();
  if (  spellCount < SPELL_START_INDEX) {
    m_hilightItem = spellCount-1;
  } else {
    m_topIndex = spellCount - SPELL_START_INDEX;
    m_hilightItem = 7;
  }
  listSpells();
  selectSpell();
}

void SpellSelect::r() {
  examineSpell();
}

void SpellSelect::examineSpell() {
  
  Text16::drawToTop();
  Arena & arena(Arena::instance());
  arena.setCursor(15,0);
  arena.setSpellAt(15, 0, Wizard::currentPlayer().spellId(m_hilightItem+m_topIndex));

  if (not m_examineScreen) {
    m_examineScreen = new ExamineSquare();
  } else {
    m_examineScreen->initialise();
  }
  m_examineScreen->showCastChance(true);
  m_examineScreen->show();
  Text16::drawToBottom();
  Video::instance(1).setFade(0);
}


void SpellSelect::a(void) {
  if (m_examine)
  {
    examineSpell();
  }
  else
  {
    chooseSpell();
  }
}

void SpellSelect::chooseSpell() {
  // store the spell...
  Wizard & player(Wizard::currentPlayer());
  int currentSpellIndex(m_hilightItem+m_topIndex);
  player.setSelectedSpell(currentSpellIndex);
  int spellId(player.spellId(currentSpellIndex));
  // check for illusion...
  if (spellId < SPELL_KING_COBRA or spellId >= SPELL_GOOEY_BLOB) 
  {
    SoundEffect::play(SND_CHOSEN);
    b();
    return;
  }
  GameState::instance().setNextScreen(new IllusionPicker());
}

void SpellSelect::b(void) {
  Video::instance(1).fade();
  Video::instance().fade();
  Text16::drawToTop();
  Text16::instance().clear();
  Arena::instance().clear();
  // remove any examined spell...
  Arena::instance().setSpellAt(15, 0, 0);
  Text16::drawToBottom();
  GameState::instance().setNextScreen(new GameMenu());
}


void SpellSelect::selectSpell() {
  // select the spell at index "item"
  
  // get the actual spell id index
  int spellIndex = m_hilightItem+m_topIndex;
  const SpellData * currentSpell = Wizard::currentPlayer().spell(spellIndex);
  
  currentSpell->printName(SPELL_NAME_X,ARROW_UP_Y+m_hilightItem*2, 10);
  
  int chance = currentSpell->realCastChance() + 1;
  Text16 & text16 = Text16::instance();
  text16.print("CASTING",     CAST_CHANCE_POS_X,CAST_CHANCE_POS_Y, 2);
  text16.print("CHANCE=    ", CAST_CHANCE_POS_X,CAST_CHANCE_POS_Y+2, 2);
  char str[30];
  Text16::int2a(chance*10, str);
  strcat(str,"/");
  text16.print(str, CAST_CHANCE_POS_X+7,16, 5);
  
  Color colour(s_castingChancePalette[chance/2]);
  colour.red(colour.red()+31);
  colour.green(colour.green()+31);
  colour.blue(colour.blue()+31);
  
  text16.setColour(10, colour);
  Graphics::instance().setAnimationParams(-15,-8);
  examineSpell();
}

void SpellSelect::deselectSpell(int item) {
  
  // get the actual spell id index
  int spellIndex = item+m_topIndex;
  const SpellData * spell = Wizard::currentPlayer().spell(spellIndex);
  if (spell)
    spell->printName(SPELL_NAME_X,ARROW_UP_Y+(item*2));
}
