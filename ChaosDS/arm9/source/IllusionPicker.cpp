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
#include "libnds.h"
#include "ndspp.h"
#include "GameMenu.h"
#include "GameState.h"
#include "HotSpot.h"
#include "IllusionPicker.h"
#include "SoundEffect.h"
#include "Text16.h"
#include "Wizard.h"

using nds::Rectangle;

static const int YES_POS_X(17);
static const int YES_POS_Y(10);

IllusionPicker::IllusionPicker():SpellSelect(false) 
{
  clearHotSpots();
  Rectangle yesRect(YES_POS_X*8, YES_POS_Y*8, 6*8, 16);
  m_hotspots.push_back(new HotSpot(yesRect, yesCb, this));
  Rectangle noRect((YES_POS_X*8)+yesRect.w, YES_POS_Y*8, 6*8, 16);
  m_hotspots.push_back(new HotSpot(noRect, noCb, this));
}

void IllusionPicker::yesCb(void * arg)
{
  IllusionPicker * self = (IllusionPicker*)arg;
  Wizard & player(Wizard::currentPlayer());
  player.setIllusionCast(true);
  self->b();
}

void IllusionPicker::noCb(void * arg)
{
  IllusionPicker * self = (IllusionPicker*)arg;
  Wizard & player(Wizard::currentPlayer());
  player.setIllusionCast(false);
  self->b();
}

void IllusionPicker::show()
{
  // ask if they want an illusion and wait for response
  Text16 & text16(Text16::instance());
  text16.setColour(14, 0);  
  text16.setColour(12, nds::Color(31,31,31));
  text16.print("ILLUSION?", 18, 8, 12);  
  text16.print("(A=YES B=NO)", YES_POS_X, YES_POS_Y, 12);

}

void IllusionPicker::handleKeys()
{
  u16 keysSlow = keysDownRepeat();
  bool choiceMade(false);

  if (keysSlow & KEY_A) {
    Wizard & player(Wizard::currentPlayer());
    player.setIllusionCast(true);
    choiceMade = true;
  } 
  else if (keysSlow & KEY_B) {
    choiceMade = true;
  } 
  if (keysSlow & KEY_TOUCH)
  {
    handleTouch();
  }
  if (choiceMade)
  {
    SoundEffect::play(SND_CHOSEN);
    b();
  }
}
