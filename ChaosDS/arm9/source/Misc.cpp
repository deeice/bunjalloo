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
#include <ctype.h>
#include <stdio.h>
#include "libnds.h"
#include "GameState.h"
#include "Misc.h"
#include "Rectangle.h"
#include "Text16.h"
#include "Wizard.h"

// default seed
static unsigned int s_random(14071977);
void Misc::seedRand(unsigned int seed) {
  s_random = seed;
}

// churn the random number
void Misc::churnRand(void) {
  s_random *= 663608941;
}

unsigned int Misc::rand(unsigned int maximum) {
  unsigned long long result = maximum;
  result *= s_random;
  churnRand();
  return (unsigned int)(result >> 32);
}

char * Misc::trim(char * str) {
  char *s = str;
  char *d = str;
  
  // eat trailing white space
  while (*s)
    s++;
  while (str < s and isspace(*(s-1)))
    *s-- = '\0';
  
  *s = '\0';
  
  s = str;
  // has leading space?
  if (isspace(*s))
  {
    // eat white space 
    while (isspace(*s))
      s++;
    
    // copy
    while (*s)
      *d++ = *s++;
    *d = '\0';
  } 
  return str;
}

// based on code at c64c
void Misc::orderTable(int count, unsigned char * table) {
  // orders "count" entries in the given table by priority
  // table contains priority, index pairs
  
  for (int i = 0; i < count; i++) {
    int index = 0;
    for (int j = 0; j < count; j++) {
      // priority 1, 2, value 1, 2
      unsigned char p1(table[index]);
      index++;  
      unsigned char val1(table[index]);
      index++;  
      unsigned char p2(table[index]);
      index++;
      unsigned char val2(table[index]);
      index--;   // points to prio 2
      if (p2 > p1) {
        index++;  // points to val 2
        table[index] = val1;
        index--;  // points to prio 2
        table[index] = p1;
        index--;  // points to val 1
        table[index] = val2;
        index--;  // points to prio 1
        table[index] = p2;
        index++;  // points to val 1
        index++;  // points to prio 2
      }
    }
  }
}

bool Misc::keypressWait()
{
    scanKeys();
    swiWaitForVBlank();
    u16 keysSlow = keysDownRepeat();
    if ( keysSlow & (KEY_A|KEY_B|KEY_L|KEY_R|
                     KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT|
                     KEY_START|KEY_SELECT|KEY_X|KEY_Y|KEY_TOUCH)) 
      return true;
    return false;
}

void Misc::delay(int time, bool callMainLoop) 
{
  GameState & state(GameState::instance());
  state.resetGameFrame();
  while(state.gameFrame() < time) 
  {
    swiWaitForVBlank();
    if (callMainLoop)
      state.mainLoopExecute();
  }
}

void Misc::waitForLetgo()
{
  if (Wizard::currentPlayer().isCpu())
    return;
  bool prsd(true);
  while (prsd) {
    swiWaitForVBlank();
    scanKeys();
    u16 keys = keysHeld();
    if (   (keys & KEY_A)
        or (keys & KEY_B)
        or (keys & KEY_L)
        or (keys & KEY_R)
        or (keys & KEY_UP)
        or (keys & KEY_DOWN)
        or (keys & KEY_LEFT)
        or (keys & KEY_RIGHT)
        or (keys & KEY_START)
        or (keys & KEY_SELECT)
       )
    {
        prsd = true;
        GameState::instance().mainLoopExecute();
    }
    else
    {
        prsd = false;
    }
  }
}

bool Misc::confirm()
{
  waitForLetgo();
  int yes_pressed = 0;
  // 012345678901234567890123456
  // DISMOUNT WIZARD? A=YES B=NO
  nds::Rectangle yesRect(17*8, Text16::MESSAGE_Y*8, 5*8, 16 );
  nds::Rectangle noRect(23*8, Text16::MESSAGE_Y*8, 4*8, 16 );
  while (yes_pressed == 0) {
    swiWaitForVBlank();
    scanKeys();
    u16 keysSlow = keysDownRepeat();
    if (keysSlow & KEY_A)
      yes_pressed = 2;
    if (keysSlow & KEY_B)
      yes_pressed = 1;
    if (keysSlow & KEY_TOUCH)
    {
      touchPosition pos;
      touchRead(&pos);
      if ( yesRect.hit(pos.px, pos.py) )
      {
        yes_pressed = 2;
      }
      else if ( noRect.hit(pos.px, pos.py) )
      {
        yes_pressed = 1;
      }
    }
  }
  waitForLetgo();
  yes_pressed--;
  return yes_pressed;
}
