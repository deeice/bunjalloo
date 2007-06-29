#include "libnds.h"
#include <string.h>
#include "ndspp.h"
#include "SoundEffect.h"
#include "Options.h"
#include "Arena.h"
#include "Graphics.h"
#include "Text16.h"
#include "Misc.h"
#include "GameState.h"
#include "Splash.h"
#include "Rectangle.h"
#include "HotSpot.h"

using namespace std;
using namespace nds;


const int Options::DEFAULT_ROUNDS = 1;
const int Options::OPTION_COUNT(BACK+1);

Option Options::s_option[] = {
 Option("Round Limit", Option::ROUND_LIMIT,0,500),
 Option("Old Bugs",Option::ON_OFF,0,1),
 Option("New Stuff",Option::ON_OFF,0,1),
 Option("Sound",Option::ON_OFF,0,1),
 Option("Sound Test",Option::NUMBER,SND_ATTACK,SND_WALK),
 Option("          BACK",Option::NONE,0,1)
};

OptionScreen::OptionScreen()
{
  // start of the text.
  int x = 3+12;
  // y pos start of options
  int y = 5;
  Option * option = Options::instance().options();
  for (int j = 0; j < Options::OPTION_COUNT; j++) { 
    char buffer[32];
    int ypos = y + j*3;
    option[j].asText(buffer);
    if (buffer[0] != 0)
    {
      // bounding boxes for increase and decrease arrows
      Rectangle decr = {x*8,ypos*8,8,16};
      Rectangle incr = {(x+8)*8,ypos*8,8,16};
      // bounding box for activate
      Rectangle actv = {(x+1)*8,ypos*8,64,16};
      // register the callbacks
      m_hotspots.push_back(new HotSpot(incr, incrOptionCb, this));
      m_hotspots.push_back(new HotSpot(decr, decrOptionCb, this));
      m_hotspots.push_back(new HotSpot(actv, activateOptionCb, this));
    }
    else
    {
      // bounding box for activate
      Rectangle actv = {(x-12)*8,ypos*8,strlen(option[j].m_name)*8,16};
      m_hotspots.push_back(new HotSpot(actv, activateOptionCb, this));
    }
  }
}

void OptionScreen::decrOptionCb(void * arg)
{
  OptionScreen * self = (OptionScreen*)arg;
  self->selectFromTouch();
  self->left();
}

void OptionScreen::incrOptionCb(void * arg)
{
  OptionScreen * self = (OptionScreen*)arg;
  self->selectFromTouch();
  self->right();
}

void OptionScreen::activateOptionCb(void * arg)
{
  OptionScreen * self = (OptionScreen*)arg;
  self->selectFromTouch();
  self->a();
}

void OptionScreen::selectFromTouch()
{
  int optionIndex = (((m_checking->area().y/8)-5)/3);
  deselectOption(m_hilightItem);
  m_hilightItem = optionIndex;
  selectOption(m_hilightItem);
}

void OptionScreen::show()
{
  Graphics::clearPalettes();
  Text16::instance().clear();
  m_hilightItem = 0;
  Graphics::instance().setAnimationParams(-31,-8);
  
  Text16::instance().print("GAME OPTIONS", 10, 2, 14);
  drawOptions();
  
  Text16::instance().setColour(14, Color(31,30,0));
  Arena::instance().decorativeBorder(15, Color(0,0,0),Color(31,0,0)); 
  
  selectOption(m_hilightItem);
  Video::instance(0).fade(false);

}


void OptionScreen::animate()
{
  Color c(0,31,0);
  Graphics::instance().animateSelection(m_hilightItem+1, c);
}

void OptionScreen::handleKeys()
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
    back();
  } 
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  }
}

void OptionScreen::drawOptions() const
{
  int x = 3;
  int y = 5;
  Option * option = Options::instance().options();
  for (int j = 0; j < Options::OPTION_COUNT; j++) { 
    drawOption(option[j], x, y+j*3, j+1);
    deselectOption(j);
  }
}

void OptionScreen::deselectOption(int item) const
{
  Text16::instance().setColour(item+1, Color(0,14,0));
}

void OptionScreen::selectOption(int item) const
{
  Text16::instance().setColour(item+1, Color(0,31,0));
}

void OptionScreen::drawOption(Option & opt, int x, int y, int pal) const
{
  // used to draw the value of a particular option after changing it...
  Text16 & text16(Text16::instance());
  text16.print(opt.m_name, x, y, pal);
  char buffer[128];
  opt.asText(buffer);
  text16.print(buffer, x+12, y, pal);
}

void OptionScreen::a()
{
  Option & opt(Options::instance().option(Options::OptionPosition_t(m_hilightItem)));
  if (m_hilightItem == Options::NEW_FEATURES)  {
#if 0
    // opt++; /* Doesn't work? Cart dependent... */
    // swi 00? FIXME
#endif
  }
  
  if (m_hilightItem == Options::BACK)
  {
    back();
  }
  
  if (m_hilightItem == Options::SOUND_TEST)  {
    SoundEffect::play(static_cast<SoundEffect_t> ( static_cast<int>(opt)));
  }
}


void OptionScreen::up(void)
{
  if (m_hilightItem > 0)
  {
    SoundEffect::play(SND_MENU);
    deselectOption(m_hilightItem);
    m_hilightItem--;
    selectOption(m_hilightItem);
  }
}

void OptionScreen::down(void)
{
  if (m_hilightItem < Options::OPTION_COUNT-1)
  {
    SoundEffect::play(SND_MENU);
    deselectOption(m_hilightItem);
    m_hilightItem++;
    selectOption(m_hilightItem);
  }
}

void OptionScreen::left(void) {

  Option & opt(Options::instance().option(Options::OptionPosition_t(m_hilightItem)));
  opt--;
  drawOption(opt, 3, 5+m_hilightItem*3, m_hilightItem+1);
    
}

void OptionScreen::right(void) {
  Option & opt(Options::instance().option(Options::OptionPosition_t(m_hilightItem)));
  opt++;
  drawOption(opt, 3, 5+m_hilightItem*3, m_hilightItem+1);
  
}

void OptionScreen::back(void) {
  SoundEffect::play(SND_CHOSEN);
  Misc::waitForLetgo();
  Video::instance().fade();
  GameState::instance().setNextScreen(new Splash());
}


// Option class
const char * const Option::ON_STRING( "ON     ");
const char * const Option::OFF_STRING("OFF    ");
const char * const Option::DEFAULT_STRING("DEFAULT");

Option::Option(const char * name, 
        Option_t type,
        unsigned short min,
        unsigned short max) :
  m_name(name),
  m_type(type),
  m_min(min),
  m_max(max)
{
  if (m_type == ON_OFF) {
    m_min = 0;
    m_max = 1;
  }
}

void Option::setValue(unsigned short value)
{
  if ((value <= m_max) and (value >= m_min))
    m_value = value;
}

unsigned short Option::operator++(int )
{
  unsigned short cache = m_value;
  if (m_value < m_max)
    m_value++;
  return cache;
}
unsigned short Option::operator--(int )
{
  unsigned short cache = m_value;
  if (m_value > m_min)
    m_value--;
  return cache;
}
Option::operator int() const
{
  return m_value;
}

Option::operator bool() const
{
  return m_value != 0;
}

void Option::asText(char * buffer) const
{
  char * tmp = buffer;
  buffer++;
  if (m_type == ON_OFF) 
  {
    if (m_value)
      strcpy(buffer, ON_STRING);
    else
      strcpy(buffer, OFF_STRING);
  } 
  else if (m_type == ROUND_LIMIT and m_value < 2) {
    if (m_value == 0) {
      strcpy(buffer, OFF_STRING);
    } else if (m_value == 1) {
      strcpy(buffer, DEFAULT_STRING);
    }
  }
  else if (m_type == NONE)
  {
    buffer[0] = 0;
    tmp[0] = 0;
  }
  else
  {
    // NUMBER...
    Text16::int2a(m_value, buffer);
    int len = strlen(buffer);
    while (len < 7) {
      buffer[len++] = ' ';
    }
    buffer[len] = 0;
  }
  if (m_type != NONE)
  {
    tmp[0] = Text16::LEFT_ARROW_INDEX;
    int len = strlen(buffer);
    buffer[len++] = Text16::RIGHT_ARROW_INDEX;
    buffer[len] = 0;
  }

}


// Options class
Options::Options()
{
  s_option[ROUND_LIMIT].setValue(DEFAULT_ROUNDS);
  s_option[OLD_BUGS].setValue(Option::ON);
  s_option[NEW_FEATURES].setValue(Option::ON);
  s_option[SOUND_ENABLE].setValue(Option::OFF);
}

Options::~Options(){}
Options & Options::instance()
{
  static Options s_instance;
  return s_instance;
}

Option & Options::option(OptionPosition_t opt) const
{
  return s_option[opt];
}

Option * Options::options() const
{
  return s_option;
}

