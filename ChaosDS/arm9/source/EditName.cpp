/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include <string.h>
#include "libnds.h"
#include "ndspp.h"
#include "Arena.h"
#include "CreatePlayers.h"
#include "EditName.h"
#include "GameState.h"
#include "Graphics.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Rectangle.h"
#include "SoundEffect.h"
#include "Text16.h"
#include "Wizard.h"

using nds::Color;
using nds::Rectangle;

static const int OK_LETTER(71);
static const int DELETE_LETTER(70);

EditName::EditName()
{
  // create one big rectangle for the keyboard and 2 smaller ones for OK and back buttons
  Rectangle lettersRect = { 2*8, 6*8, 25*8, 16*5};
  m_hotspots.push_back(new HotSpot(lettersRect, letterCb, this));

  Rectangle okRect = { 14*8, 16*8, 32, 16 };
  m_hotspots.push_back(new HotSpot(okRect, okCb, this));

  Rectangle deleteRect = { 25*8, 16*8, 16, 16};
  m_hotspots.push_back(new HotSpot(deleteRect, deleteCb, this));

}

void EditName::show()
{
  // the edit name screen... 
  Text16 & text16 = Text16::instance();
  Arena & arena = Arena::instance();
  text16.clear();
  arena.clear();
  Graphics::instance().clearPalettes();
  strcpy(m_name, Wizard::currentPlayer().name());
  m_hilightItem = OK_LETTER;
  
  // draw the alphabet...
  updateName();
  
  char backspace[2] = {Text16::LEFT_ARROW_INDEX, 0};
  text16.print("A B C D E F G H I J K L M", 2,6, 10);
  text16.print("N O P Q R S T U V W X Y Z", 2,8, 10);
  text16.print("                         ", 2,10, 10);
  text16.print("a b c d e f g h i j k l m", 2,12, 10);
  text16.print("n o p q r s t u v w x y z", 2,14, 10);
  text16.print("OK", 15,16, 13);
  text16.print(backspace,                  26,16, 10);
  
  text16.setColour(10, Color(0,30,30)); // l blue
  text16.setColour(11, Color(31,30,30)); // white
  text16.setColour(12, Color(31,30,0)); // yellow
  text16.setColour(13, Color(31,30,30)); // white
  
  selectLetter();
  
  arena.decorativeBorder(15, Color(0,0,31),Color(0,31,31)); 
  nds::Video::instance().fade(false);
}

// convert a hilite index to the letter and x, y position
void EditName::indexToPosition(int & x, int &y, char & c)
{
  // 0 - 26 = A - Z
  // 27 - 52 = a - z
  // position = A = (2,6) B = (4,6)... M = 26,6
  // i.e. A-M = (letter - startx * 2), 6
  if (m_hilightItem <= 12) {
    x = (m_hilightItem)<<1;
    y = 6;
    c = 'A'+m_hilightItem;
  }
  if (m_hilightItem  >= 13 && m_hilightItem <= 25) {
    x = (m_hilightItem - 13)<<1;
    y = 8;
    c = 'N'+m_hilightItem-13;
  }
  if (m_hilightItem  >= 26 && m_hilightItem <= 38) {
    x = (m_hilightItem - 26)<<1;
    y = 10;
    c = '_';
  }
  
  if (m_hilightItem  >= 39 && m_hilightItem <= 51) {
    x = (m_hilightItem - 39)<<1;
    y = 12;
    c = 'a'+m_hilightItem-39;
  }
  if (m_hilightItem  >= 52 && m_hilightItem <= 64) {
    x = (m_hilightItem - 52)<<1;
    y = 14;
    c = 'n'+m_hilightItem-52;
  }
  if (m_hilightItem == DELETE_LETTER) {
    x = 24;
    y = 16;
    c = Text16::LEFT_ARROW_INDEX;
  }
  x += 2;
}

void EditName::selectLetter()
{
  if (m_hilightItem == OK_LETTER) {
    // OK hilited
    Text16::instance().setColour(13, Color(0,30,0)); // white
  } else {
    int x = 2;
    int y = 6;
    char c;
    indexToPosition(x, y, c);
    char str[3];
    str[0] = c;
    str[1] = 0;
    Text16::instance().print(str, x,y, 11);
  }
  
}

void EditName::deselectLetter() {
  
  if (m_hilightItem == OK_LETTER) {
    // OK hilited
    Text16::instance().setColour(13, Color(31,30,30)); // white
  }
  else {
    int x = 2;
    int y = 6;
    char c;
    indexToPosition(x, y, c);
    char str[3];
    if (c == '_')
      c = ' ';
    str[0] = c;
    str[1] = 0;
    Text16::instance().print(str, x,y, 10);
  }
  
}

void EditName::up()
{
  // move up a row...
  deselectLetter();
  if (m_hilightItem > 12) {
    if (m_hilightItem == DELETE_LETTER)
    {
      m_hilightItem -= 6;
    }
    else {
      m_hilightItem -= 13;
    }
  }
  else {
    m_hilightItem = OK_LETTER;
  }
  selectLetter();
  
}

  
void EditName::down()
{
  // move down a row...
  deselectLetter();
  if (m_hilightItem < 52) {
    m_hilightItem += 13;
  }
  else {
    if (m_hilightItem > 60 and m_hilightItem != OK_LETTER) {
      m_hilightItem = DELETE_LETTER;
    }
    else
    {
      //m_hilightItem -= 13*4;
      // select OK
      m_hilightItem = OK_LETTER;
    }
  }
  selectLetter();
  
}

void EditName::left(){
  
  deselectLetter();
  if (m_hilightItem == OK_LETTER) {
    return;
  }
  if (m_hilightItem == DELETE_LETTER)
  {
    m_hilightItem = OK_LETTER;
    selectLetter();
    return;
  }
  
  // get the multiple of 13..
  int this_row_index = m_hilightItem; 
  while (this_row_index - 13 >= 0) {
    this_row_index -= 13;
  }
  
  if (this_row_index > 0) {
    // not on the zeroth column
    m_hilightItem --;
  }
  else {
    // on the zeroth col, so move to the end of this row
    m_hilightItem += 12;
  }
  selectLetter();
}

void EditName::right()
{
  deselectLetter();
  if (m_hilightItem == OK_LETTER) {
    m_hilightItem = DELETE_LETTER;
    selectLetter();
    return;
  }
  if (m_hilightItem == DELETE_LETTER)
  {
    return;
  }
  
  
  // get the multiple of 12..
  int this_row_index = m_hilightItem; 
  while (this_row_index - 13 >= 0) {
    this_row_index -= 13;
  }
  
  if (this_row_index < 12) {
    m_hilightItem++;
  }
  else {
    m_hilightItem -= 12;
  }
  selectLetter();
}

void EditName::a()
{
  // add the currently selected character to the name
  if (m_hilightItem == OK_LETTER) {
    // OK hilited...
    start();
  }
  else if (m_hilightItem == DELETE_LETTER)
  {
    b();
  }
  else {
    int x, y;
    char c;
    indexToPosition(x, y, c);
    if (c == '_')
      c = ' ';
    int len = strlen(m_name);
    if (len < 11) {
      m_name[len] = c;
      m_name[len+1] = 0;
//      sprintf(m_name, "%s%c", m_name, c);
      SoundEffect::play(SND_MENU);
    }
    updateName();
  }
  
}


void EditName::start() {
  Misc::trim(m_name);
  int len = strlen(m_name);
  if (len > 0) {
    // name is valid
    SoundEffect::play(SND_SPELLSUCCESS);
    Wizard::currentPlayer().setName(m_name);
    nds::Video::instance().fade();
    GameState::instance().setNextScreen(new CreatePlayers(false));
  }
}


void EditName::b()
{
  // delete a letter...
  int len = strlen(m_name);
  if (len > 0) {
    m_name[len-1] = 0;
    SoundEffect::play(SND_CHOSEN);
  }
  updateName();
}

void EditName::updateName()
{
  Text16 & text16(Text16::instance());
  text16.print("             ", 4,2, 12);
  text16.print(m_name, 4,2, 12);
  int len = strlen(m_name);
  if (len < 11) {
    text16.print("_", 4+len,2, 12);
  }
}

void EditName::animate()
{
  if (m_hilightItem == OK_LETTER) {
    Color c(0,31,0);
    Graphics::instance().animateSelection(13, c);
  } else {
    Color c(0,31,31);
    Graphics::instance().animateSelection(11, c);
  }
}

void EditName::handleKeys()
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
  if (keysSlow & KEY_B) {
    b();
  } 
  if (keysSlow & KEY_A) {
    a();
  } 
  if (keysSlow & KEY_START) {
    start();
  } 
  if (keysSlow & KEY_TOUCH) {
    handleTouch();
  } 
}

void EditName::letterCb(void * arg)
{
  EditName * self = (EditName*)arg;
  self->deselectLetter();
  // get the selected letter from the x/y position
  int x = (self->m_x - self->m_checking->area().x) /16;
  int y = (self->m_y - self->m_checking->area().y)/16;
  self->m_hilightItem = x+y*13;
  self->selectLetter();
  self->a();

}

void EditName::okCb(void * arg)
{
  EditName * self = (EditName*)arg;
  self->m_hilightItem = OK_LETTER;
  self->a();
}

void EditName::deleteCb(void * arg)
{
  EditName * self = (EditName*)arg;
  self->m_hilightItem = DELETE_LETTER;
  self->b();
}

