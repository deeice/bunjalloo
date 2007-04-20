#include "nds.h"
#include "Canvas.h"
#include "Background.h"
#include "Video.h"

using namespace nds;

Canvas & Canvas::instance()
{
  static Canvas s_canvas;
  return s_canvas;
}

Canvas::~Canvas(){}

Canvas::Canvas():
  m_bgMain(new Background(0,3,0,31)),
  m_bgSub(new Background(1,3,0,31))
{
  // set up background.
  Video & main(Video::instance());
  Video & sub(Video::instance(1));
  sub.setMode(5);
  main.setMode(5);
  m_bgMain->enable();
  m_bgSub->enable();

  m_bgMain->update();
  m_bgSub->update();
}

int Canvas::height() const
{
  // from nds/arm9/video.h
  return SCREEN_HEIGHT*2;
}
int Canvas::width() const
{
  // from nds/arm9/video.h
  return SCREEN_WIDTH;
}

void Canvas::drawPixel(int x, int y, int colour)
{
  if (x >= SCREEN_WIDTH or y >= (2*SCREEN_HEIGHT)) {
    return;
  }
  uint16 * gfx( (y < 192) ? BG_GFX:BG_GFX_SUB );
  if (y >= 192)
    y -= 192;
  gfx[x+y*256] = colour|BIT(15);
}

void Canvas::fillRectangle(int x, int y, int w, int h, int colour)
{
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      drawPixel(x+i, y+j, colour);
    }
  }
}

void Canvas::verticalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x, y+i, colour);
  }
}
void Canvas::horizontalLine(int x, int y, int length, int colour)
{
  for (int i = 0 ; i < length; ++i)
  {
    drawPixel(x+i, y, colour);
  }
}
void Canvas::drawRectangle(int x, int y, int w, int h, int colour)
{
  horizontalLine(x,y,w,colour);
  horizontalLine(x,y+h,w,colour);
  verticalLine(x,y,h,colour);
  verticalLine(x+w,y,h,colour);
}
