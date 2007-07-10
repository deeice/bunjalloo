#include "libnds.h"
#include "ndspp.h"
#include "ArenaTouchScreen.h"
#include "Arena.h"
#include "Text16.h"
#include "HotSpot.h"
#include "Options.h"
#include "ExamineSquare.h"

using nds::Rectangle;
using nds::Video;

ArenaTouchScreen::ArenaTouchScreen():m_examineScreen(0)
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

void ArenaTouchScreen::deleteScreen()
{
  if (m_examineScreen) {
    Video::instance(1).fade();
    delete m_examineScreen;
    m_examineScreen = 0;
  }
}

void ArenaTouchScreen::examine() 
{

  // if examine the same square again, call "next" instead
  if (m_examineScreen and Arena::instance().targetIndex() == m_examineScreen->index())
  {
    m_examineScreen->handleKeys();
    return;
  }

  int theCreature = Arena::instance().cursorContents();
  // remove current examining screen if we examine an empty square
  if (theCreature == 0) {
    deleteScreen();
    return;
  }
  if (m_examineScreen) {
    deleteScreen();
  }
  else {
    Video::instance(1).setFade(16);
  }
  m_examineScreen = new ExamineSquare();
  Text16::drawToTop();
  m_examineScreen->show();
  Text16::drawToBottom();
  Video::instance(1).fade(false);
}

