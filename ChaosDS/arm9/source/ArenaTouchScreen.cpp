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
#include "Arena.h"
#include "ArenaTouchScreen.h"
#include "ExamineSquare.h"
#include "HotSpot.h"
#include "Misc.h"
#include "Options.h"
#include "Text16.h"

using nds::Rectangle;
using nds::Video;

ArenaTouchScreen::ArenaTouchScreen():m_examineScreen(new ExamineSquare)
{
  Rectangle screenRect = {Arena::POSITION_X, Arena::POSITION_Y, Arena::PIXEL_WIDTH, Arena::PIXEL_HEIGHT};
  m_hotspots.push_back(new HotSpot(screenRect, arenaCb, this));

  Rectangle examineRect = { 0,  Text16::MESSAGE_Y*8, SCREEN_WIDTH-32, 16};
  m_hotspots.push_back(new HotSpot(examineRect, examineCb, this));

  Rectangle endRect = { SCREEN_WIDTH - 16,  Text16::MESSAGE_Y*8, 8, 16};
  m_hotspots.push_back(new HotSpot(endRect, exitCb, this));
}

ArenaTouchScreen::~ArenaTouchScreen()
{
  delete m_examineScreen;
}

void ArenaTouchScreen::exitCb(void * arg)
{
  ArenaTouchScreen * self = (ArenaTouchScreen*)arg;
  self->next();
}
void ArenaTouchScreen::examineCb(void * arg)
{
  ArenaTouchScreen * self = (ArenaTouchScreen*)arg;
  if (self->m_examineScreen) {
    self->m_examineScreen->handleKeys();
  }
}

void ArenaTouchScreen::arenaCb(void * arg)
{
  ArenaTouchScreen * self = (ArenaTouchScreen*)arg;
  Arena & arena(Arena::instance());
  int tileX, tileY, x, y;
  self->lastTouchToTile16(tileX, tileY);
  arena.targetXY(x, y);
  x--; y--;
  if (tileX == x and tileY == y)
  {
    self->execute();
  }
  else
  {
    arena.setCursor(tileX, tileY);
    if (Options::instance().option(Options::ONE_TOUCH))
    {
      self->execute();
    }
  }
}

void ArenaTouchScreen::examine() 
{

  // if examine the same square again, call "next" instead
  if (not m_examineScreen->isHidden() and Arena::instance().targetIndex() == m_examineScreen->index())
  {
    m_examineScreen->handleKeys();
    return;
  }

  int theCreature = Arena::instance().cursorContents();
  // remove current examining screen if we examine an empty square
  if (theCreature == 0) {
    m_examineScreen->hide();
    return;
  }
  m_examineScreen->hide();
  Text16::drawToTop();
  m_examineScreen->initialise();
  m_examineScreen->show();
  Text16::drawToBottom();
}

