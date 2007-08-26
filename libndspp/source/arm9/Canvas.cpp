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
#include <string.h>
#include "nds.h"
#include "Canvas.h"
#include "Background.h"
#include "Video.h"

using namespace nds;

static const int BUFFER_SIZE = 256*256;

Canvas & Canvas::instance()
{
  static Canvas s_canvas;
  return s_canvas;
}

Canvas::~Canvas(){}

Canvas::Canvas():
  m_bgMain(new Background(0,3,0,31)),
  m_bgSub(new Background(1,3,0,31)),
  m_frontMain((unsigned short*)BG_BMP_RAM(0)),
  m_backMain( m_frontMain + BUFFER_SIZE ),
  m_frontSub((unsigned short*)BG_BMP_RAM_SUB(0)),
  m_backSub((unsigned short*)BG_BMP_RAM(16))
{
  // set up background.
  Video & main(Video::instance());
  Video & sub(Video::instance(1));
  sub.setMode(5);
  main.setMode(5);

  // redo the banks - we want enough VRAM for the main and sub screen
  // Can't use hardware double buffering on sub screen, have to fudge it.
  vramSetMainBanks( VRAM_A_MAIN_BG_0x06000000,
                    VRAM_B_MAIN_BG_0x06020000,
                    VRAM_C_SUB_BG,
                    VRAM_D_MAIN_BG_0x06040000);
  /*
  vramSetBankI(VRAM_I_SUB_SPRITE);
  vramSetBankF(VRAM_F_MAIN_SPRITE);
  vramSetBankG(VRAM_G_MAIN_SPRITE);
  */

  m_bgMain->enable();
  m_bgSub->enable();

  m_bgMain->update();
  m_bgSub->update();

  main.setToTop();
  m_clip.x = m_clip.y = 0;
  m_clip.w = width();
  m_clip.h = height();
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
  if (not m_clip.hit(x, y))
    return;
  //uint16 * gfx( (y < 192) ? m_backMain:m_backSub );
  uint16 * gfx( vram(y));
  if (y >= 192)
    y -= 192;
  gfx[x+y*256] = colour|BIT(15);
}

unsigned short * Canvas::vram(int y)
{
  return (y < 192) ? m_backMain:m_backSub;
}

void Canvas::endPaint()
{
  // swap main screen
  unsigned short* temp = m_frontMain;
  m_frontMain = m_backMain;
  m_backMain = temp;
  // flip 
  // base is 16KB and screen size is 256x256x2 (128KB)
  BG3_CR ^= BG_BMP_BASE( 128 / 16 );

  // copy sub screen - buffer size * 2 since is 16 bit
  //memcpy(m_frontMain, m_backMain, SCREEN_WIDTH*SCREEN_HEIGHT*2);
  memcpy(m_frontSub, m_backSub, SCREEN_WIDTH*SCREEN_HEIGHT*2);

  memcpy(m_backMain, m_frontMain, SCREEN_WIDTH*SCREEN_HEIGHT*2);
}
