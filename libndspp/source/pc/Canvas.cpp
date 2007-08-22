#include "Canvas.h"
#include "Video.h"
#include "SDLhandler.h"

using namespace nds;

Canvas & Canvas::instance()
{
  static Canvas s_canvas;
  return s_canvas;
}

Canvas::~Canvas(){}

Canvas::Canvas()
{
  Video & main(Video::instance());
  Video & sub(Video::instance(1));
  sub.setMode(5);
  main.setMode(5);
}

int Canvas::height() const
{
  return SDLhandler::HEIGHT;
}
int Canvas::width() const
{
  return SDLhandler::WIDTH;
}

void Canvas::drawPixel(int x, int y, int colour)
{
  int layer = ( (y < 192) ? 0:1 );
  u16 * vram = SDLhandler::instance().vramMain(0);
  if (layer) {
    y-=192;
    vram = SDLhandler::instance().vramSub(0);
  }
  // SDLhandler::instance().drawPixel(x,y,layer,colour);
  vram[x+y*SCREEN_WIDTH] = colour;
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

void Canvas::endPaint()
{
  // nop
}
