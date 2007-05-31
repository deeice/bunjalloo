#include "libnds.h"
#include "ArenaTouchScreen.h"
#include "Arena.h"
#include "Text16.h"
#include "HotSpot.h"

using nds::Rectangle;

ArenaTouchScreen::ArenaTouchScreen()
{
  Rectangle screenRect = {Arena::POSITION_X, Arena::POSITION_Y, Arena::PIXEL_WIDTH, Arena::PIXEL_HEIGHT};
  m_hotspots.push_back(new HotSpot(screenRect, arenaCb, this));

  Rectangle examineRect = { 0,  Text16::MESSAGE_Y*8, SCREEN_WIDTH-32, 16};
  m_hotspots.push_back(new HotSpot(examineRect, examineCb, this));

  Rectangle endRect = { SCREEN_WIDTH - 16,  Text16::MESSAGE_Y*8, 8, 16};
  m_hotspots.push_back(new HotSpot(endRect, exitCb, this));
}

void ArenaTouchScreen::exitCb(void * arg)
{
  ArenaTouchScreen * self = (ArenaTouchScreen*)arg;
  self->next();
}
void ArenaTouchScreen::examineCb(void * arg)
{
  ArenaTouchScreen * self = (ArenaTouchScreen*)arg;
  self->examine();
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
  }
}
