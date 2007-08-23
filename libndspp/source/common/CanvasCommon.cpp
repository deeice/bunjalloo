#include "Canvas.h"
using namespace nds;

void Canvas::setClip(const Rectangle & clip)
{
  m_clip = clip;
  if (m_clip.x < 0)
  {
    m_clip.x = 0;
  }
  if (m_clip.y < 0)
  {
    m_clip.y = 0;
  }
  if (m_clip.w > width())
  {
    m_clip.w = width();
  }
  if (m_clip.h > height())
  {
    m_clip.h = height();
  }

}

Rectangle Canvas::clip() const
{
  return m_clip;
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

void Canvas::fillRectangle(int x, int y, int w, int h, int colour)
{
  for (int j = 0; j < h; j++) {
    horizontalLine(x, y+j, w, colour);
  }
}


