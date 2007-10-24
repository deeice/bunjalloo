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
#include "Background.h"
#include "Video.h"
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include <stdio.h>

using namespace nds;

#define BG_SIZE(m)		((m<<14))
Background::Background(
    int screen,
    int number,
    int charBlock, 
    int screenBlock, 
    int priority): 
  m_DISPCNT(screen?SUB_DISPLAY_CR:DISPLAY_CR)
{

  m_bg.screen = screen;
  m_bg.number = number;
  m_bg.charBaseBlock = charBlock;
  m_bg.screenBaseBlock = screenBlock;
  m_bg.flags = BG_SIZE(0) | BG_16_COLOR | priority;

  m_bg.PA = 1<<8;
  m_bg.PB = 0;
  m_bg.PC = 0;
  m_bg.PD = 1<<8;
  m_bg.DX = 0;
  m_bg.DY = 0;
  updateTileMapData();
}
Background::~Background()
{
  enable(false);
}

void Background::updateTileMapData()
{
  if (m_bg.screen) {
    m_bg.tileData = (unsigned short*)CHAR_BASE_BLOCK_SUB(m_bg.charBaseBlock);
    m_bg.mapData = (unsigned short*)SCREEN_BASE_BLOCK_SUB(m_bg.screenBaseBlock);
  } else {
    m_bg.tileData = (unsigned short*)CHAR_BASE_BLOCK(m_bg.charBaseBlock);
    m_bg.mapData = (unsigned short*)SCREEN_BASE_BLOCK(m_bg.screenBaseBlock);
  }
}

unsigned char Background::charBaseBlock() const
{
  return m_bg.charBaseBlock;
};
void Background::charBaseBlock(unsigned char var)
{
  if (var > 31) return;
  m_bg.charBaseBlock = var;
  updateTileMapData();
};
unsigned char Background::color() const
{
  return (m_bg.flags & BG_256_COLOR)?256:16;
};
void Background::color(unsigned char var)
{
  switch(var) {
    case 16:
      m_bg.flags &= ~BG_256_COLOR;
      m_bg.flags |= BG_16_COLOR;
      break;
    case 256:
      m_bg.flags &= ~BG_16_COLOR;
      m_bg.flags |= BG_256_COLOR;
      break;
    default:
      break;
  } 
};

static inline unsigned long BG_ON(int n) {
  return (1 << (8+n));
};

void Background::enable(bool enable)
{
  if (enable)
  {
    // this depends on if the BG is Sub or or regular BG.
    m_DISPCNT |= BG_ON(m_bg.number);

    if (Video::instance(m_bg.screen).mode() != 5) {
      int bg_ctrl = BG_MAP_BASE(m_bg.screenBaseBlock) | BG_TILE_BASE(m_bg.charBaseBlock) | m_bg.flags;
      if (m_bg.screen) {
        BGCTRL_SUB[m_bg.number] = bg_ctrl;
      } else {
        BGCTRL[m_bg.number] = bg_ctrl;
      }
    } else {
      if (m_bg.screen) {
        BGCTRL_SUB[m_bg.number] = BG_BMP16_256x256;
      } else {
        BGCTRL[m_bg.number] = BG_BMP16_256x256;
      }
    }
  }
  else
  {
    m_DISPCNT &= ~BG_ON(m_bg.number);
  }
}

bool Background::enabled() const
{
  return (m_DISPCNT & BG_ON(m_bg.number));
}

unsigned short Background::flags() const
{
  return m_bg.flags;
};
void Background::flags(unsigned short var)
{
  m_bg.flags = var;// &(3|BG_MOSAIC_ON|BG_16_COLOR|BG_256_COLOR|BG_WRAP_ON|);
};
unsigned char Background::number() const
{
  return m_bg.number;
};
void Background::number(unsigned char var)
{
  if (var > 3) return;
  m_bg.number = var;
};
unsigned char Background::screenBaseBlock() const
{
  return m_bg.screenBaseBlock;
};
void Background::screenBaseBlock(unsigned char var)
{
  if (var > 31) return;
  m_bg.screenBaseBlock = var;
  updateTileMapData();
};
Background::SIZE Background::size() const
{
  return static_cast<Background::SIZE>((m_bg.flags&BG_SIZE(3))>>14);
};
void Background::size(Background::SIZE var)
{
  unsigned int i = static_cast<unsigned int>(var);
  if (i > 3)
    return;
  m_bg.flags &= ~BG_SIZE(3);
  m_bg.flags |= BG_SIZE(i);
};

void Background::setRotateFlags(volatile unsigned short * BG_REG)
{
  s16 * src = &m_bg.PA;
  for (int i = 0 ; i < 4; ++i)
    BG_REG[i] = *src++;
  u32 * bg_reg = (u32*)&BG_REG[4];
  bg_reg[0] = m_bg.DX;
  bg_reg[1] = m_bg.DY;
}

void Background::update()
{
  vuint16 * BG_REG( m_bg.screen?&SUB_BG0_X0:&BG0_X0);
  switch(m_bg.number)
  {
    case 0: 
      /* fall through */
    case 1:
      {
        BG_REG[(m_bg.number*2)] = m_bg.xScroll;
        BG_REG[(m_bg.number*2)+1] = m_bg.yScroll;
      }
      break;
    case 2:
      {
        if(m_DISPCNT & (MODE_1_2D| MODE_2_2D|MODE_3_2D|MODE_4_2D|MODE_5_2D))
        {
          setRotateFlags(&BG_REG[8]);
        }
        else  //it is a text background
        {
          BG_REG[(m_bg.number*2)] = m_bg.xScroll;
          BG_REG[(m_bg.number*2)+1] = m_bg.yScroll;
        }
      }
      break;
    case 3:
      {
        if(m_DISPCNT & (MODE_2_2D|MODE_3_2D|MODE_4_2D|MODE_5_2D))//it is a rot background
        {
          setRotateFlags(&BG_REG[16]);
        }
        else //it is a text background
        {
          BG_REG[(m_bg.number*2)] = m_bg.xScroll;
          BG_REG[(m_bg.number*2)+1] = m_bg.yScroll;
        }
      }
      break;
    default: break;
  }
  // update flags
  if (enabled())
    enable();
}
bool Background::wraparound() const
{
  return (m_bg.flags&BG_WRAP_ON)!=0;
};
void Background::wraparound(bool var)
{
  m_bg.flags &= ~BG_WRAP_ON;
  m_bg.flags |= var?BG_WRAP_ON:0;
};
bool Background::mosaic() const
{
  return (m_bg.flags&BG_MOSAIC_ON)!=0;
};
void Background::mosaic(bool var)
{
  m_bg.flags &= ~BG_MOSAIC_ON;
  m_bg.flags |= var?BG_MOSAIC_ON:0;
};
signed short Background::xScroll() const
{
  return m_bg.xScroll;
};
void Background::xScroll(signed short var)
{
  m_bg.xScroll = var;
};
signed short Background::yScroll() const
{
  return m_bg.yScroll;
};
void Background::yScroll(signed short var)
{
  m_bg.yScroll = var;
};
signed int Background::DX() const
{
  return m_bg.DX;
};
void Background::DX(signed int var)
{
  m_bg.DX = var;
};
signed int Background::DY() const
{
  return m_bg.DY;
};
void Background::DY(signed int var)
{
  m_bg.DY = var;
};
signed short Background::PA() const
{
  return m_bg.PA;
};
void Background::PA(signed short var)
{
  m_bg.PA = var;
};
signed short Background::PB() const
{
  return m_bg.PB;
};
void Background::PB(signed short var)
{
  m_bg.PB = var;
};
signed short Background::PC() const
{
  return m_bg.PC;
};
void Background::PC(signed short var)
{
  m_bg.PC = var;
};
signed short Background::PD() const
{
  return m_bg.PD;
};
void Background::PD(signed short var)
{
  m_bg.PD = var;
};


unsigned short * Background::mapData() const
{
  return m_bg.mapData;
}
unsigned short * Background::tileData() const
{
  return m_bg.tileData;
}
