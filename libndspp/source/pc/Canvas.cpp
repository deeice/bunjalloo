/*
  Copyright (C) 2007,2008 Richard Quirk

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

Canvas::Canvas():m_frontSub(0), m_backSub(0)
{
  m_frontSub = SDLhandler::instance().vramSub(0);
  m_backSub = new unsigned short[0x16000];
  init();
}

void Canvas::init()
{
  Video & main(Video::instance());
  Video & sub(Video::instance(1));
  sub.setMode(5);
  main.setMode(5);
  main.setToBottom();
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
#ifdef ZIPIT_Z2
  int layer = ( (y < SCREEN_HEIGHT) ? 0:1 );
  u16 * vram = m_backSub;
  if (layer) {
    y-=SCREEN_HEIGHT;
    vram = SDLhandler::instance().vramMain(0);
  }
#else
  int layer = ( (y < 192) ? 0:1 );
  u16 * vram = m_backSub;
  if (layer) {
    y-=192;
    vram = SDLhandler::instance().vramMain(0);
  }
#endif
  vram[x+y*SCREEN_WIDTH] = colour;
}

unsigned short * Canvas::vram(int y)
{
#ifdef ZIPIT_Z2
  int layer = ( (y < SCREEN_HEIGHT) ? 1:0 );
#else
  int layer = ( (y < 192) ? 1:0 );
#endif
  u16 * vram = SDLhandler::instance().vramMain(0);
  if (layer) {
    vram = m_backSub;
  }
  return vram;
}

void Canvas::endPaint()
{
  SDLhandler::instance().swapMainBuffer();
#ifdef ZIPIT_Z2
# if 1
  // Try without this to see if sub is always the top screen.
  // Yes, but I need to render sub because of scroll.
  // Depending on direction it may copy pixels from sub to main.
  memcpy(m_frontSub, m_backSub, SCREEN_WIDTH*SCREEN_HEIGHT*2);
# endif
#else
  memcpy(m_frontSub, m_backSub, SCREEN_WIDTH*SCREEN_HEIGHT*2);
#endif
}

unsigned short * Canvas::frontVram(int y)
{
#ifdef ZIPIT_Z2
  int layer(y<SCREEN_HEIGHT?1:0);
#else
  int layer(y<192?1:0);
#endif
  if (layer) {
    return m_frontSub;
  }
  SDLhandler::instance().swapMainBuffer();
  m_frontMain = SDLhandler::instance().vramMain(0);
  SDLhandler::instance().swapMainBuffer();
  return m_frontMain;
}
