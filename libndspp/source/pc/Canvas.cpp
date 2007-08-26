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
  m_clip.x = m_clip.y = 0;
  m_clip.w = width();
  m_clip.h = height();
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
  if (not m_clip.hit(x, y))
    return;
  int layer = ( (y < 192) ? 0:1 );
  u16 * vram = SDLhandler::instance().vramMain(0);
  if (layer) {
    y-=192;
    vram = SDLhandler::instance().vramSub(0);
  }
  vram[x+y*SCREEN_WIDTH] = colour;
}

unsigned short * Canvas::vram(int y)
{
  int layer = ( (y < 192) ? 0:1 );
  u16 * vram = SDLhandler::instance().vramMain(0);
  if (layer) {
    vram = SDLhandler::instance().vramSub(0);
  }
  return vram;
}

/*
void Canvas::fillRectangle(int x, int y, int w, int h, int colour)
{
  if (y < 192 and (y+h) >= 192)
  {
    // crosses boundary
  }
  else if (y < 192)
  {
    u16 * vram = SDLhandler::instance().vramMain(0);
    vram += x + y * SCREEN_WIDTH;
    if ( (x+w) > (m_clip.x + m_clip.w))
    {
      w = m_clip.w - x - m_clip.x;
    }
    if ( (y+h) > (m_clip.y + m_clip.h))
    {
      h = m_clip.h - y - m_clip.y;
    }
    for (int i = 0; i < h ; ++i) {
      u16 * end = vram+w;
      while (vram != end)
      {
        *vram++ = colour;
      }
      vram += width() - w;
    }
  }
  else
  {
  }
}
*/

void Canvas::endPaint()
{
  // nop
}
