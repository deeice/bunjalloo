/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string.h>
#include "libnds.h"
#include "ndspp.h"
#include "Arena.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Options.h"
#include "Rectangle.h"
#include "SoundEffect.h"
#include "Splash.h"
#include "Text16.h"

using namespace std;
using namespace nds;

static const int LINES_PER_OPTION(2);
static const int OPTION_VALUE_X(15);
static const int OPTION_MARGIN_X(4);
static const int OPTION_MARGIN_Y(5);
const int Options::DEFAULT_ROUNDS = 1;
const int Options::OPTION_COUNT(BACK+1);

Option Options::s_option[] = {
 Option("Round Limit", Option::ROUND_LIMIT,0,500),
 Option("Old Bugs",Option::ON_OFF,0,1),
 Option("New Features",Option::ON_OFF,0,1),
 Option("One Touch",Option::ON_OFF,0,1),
 Option("Sound",Option::ON_OFF,0,1),
 Option("Sound Test",Option::NUMBER,SND_ATTACK,SND_WALK),
 Option("          BACK",Option::NONE,0,1)
};

OptionScreen::OptionScreen()
{
  // start of the text.
  int x = OPTION_MARGIN_X+OPTION_VALUE_X;
  // y pos start of options
  int y = OPTION_MARGIN_Y;
  Option * option = Options::instance().options();
  for (int j = 0; j < Options::OPTION_COUNT; j++) { 
    char buffer[32];
    int ypos = y + j*LINES_PER_OPTION;
    option[j].asText(buffer);
    if (buffer[0] != 0)
    {
      // bounding boxes for increase and decrease arrows
      Rectangle decr(x*8,ypos*8,8,16);
      Rectangle incr((x+8)*8,ypos*8,8,16);
      // bounding box for activate
      Rectangle actv((x+1)*8,ypos*8,64,16);
      // register the callbacks
      m_hotspots.push_back(new HotSpot(incr, incrOptionCb, this));
      m_hotspots.push_back(new HotSpot(decr, decrOptionCb, this));
      m_hotspots.push_back(new HotSpot(actv, activateOptionCb, this));
    }
    else
    {
      // bounding box for activate
      Rectangle actv((x-OPTION_VALUE_X)*8,ypos*8,strlen(option[j].m_name)*8,16);
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
  int optionIndex = (((m_checking->area().y/8)-OPTION_MARGIN_Y)/LINES_PER_OPTION);
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
  int x = OPTION_MARGIN_X;
  int y = OPTION_MARGIN_Y;
  Option * option = Options::instance().options();
  for (int j = 0; j < Options::OPTION_COUNT; j++) { 
    drawOption(option[j], x, y+j*LINES_PER_OPTION, j+1);
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
  text16.print(buffer, x+OPTION_VALUE_X, y, pal);
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
  drawOption(opt, OPTION_MARGIN_X, OPTION_MARGIN_Y+m_hilightItem*LINES_PER_OPTION, m_hilightItem+1);
    
}

void OptionScreen::right(void) {
  Option & opt(Options::instance().option(Options::OptionPosition_t(m_hilightItem)));
  opt++;
  drawOption(opt, OPTION_MARGIN_X, OPTION_MARGIN_Y+m_hilightItem*LINES_PER_OPTION, m_hilightItem+1);
  
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

