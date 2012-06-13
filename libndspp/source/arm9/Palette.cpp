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
#include "Palette.h"
#include <nds/arm9/video.h>
#include <nds/dma.h>
#include <nds/memory.h>

using namespace nds;

Color::Color(unsigned short s):m_c(s)
{}
Color::Color(int r, 
    int g, 
    int b):m_c(0)
{ 
  set(r,g,b);
}

void Color::set(int red, int green, int blue)
{
  if (red > 31) red = 31;
  if (green > 31) green = 31;
  if (blue > 31) blue = 31;
  if (red < 0) red = 0;
  if (green < 0) green = 0;
  if (blue < 0) blue = 0;
  
  m_c = RGB5(red&31,green&31, blue&31);
}

////////////////////////////////////////////////////////////////////////////////

Palette::Palette(int screen):
m_p(screen?BG_PALETTE_SUB:BG_PALETTE),m_16(false)
{ }

Palette::Palette(int screen, int palette):
m_p(&(screen?BG_PALETTE_SUB:BG_PALETTE)[(palette&15)*16]),m_16(true)
{ }

void Palette::load(const unsigned short * palette, unsigned int length)
{
  if (!length)
    length = m_16?16:256;
  dmaCopy(palette, m_p, length * 2);
}

void Palette::clear()
{
  int length = m_16?16:256;
  for (int i = 0; i < length; ++i)
    m_p[i] = 0;
}

////////////////////////////////////////////////////////////////////////////////

ObjectPalette::ObjectPalette(int screen):Palette(screen)
{
  m_p = screen?SPRITE_PALETTE_SUB:SPRITE_PALETTE;
}

ObjectPalette::ObjectPalette(int screen, int palette):Palette(screen, palette)
{
  m_p = &(screen?SPRITE_PALETTE_SUB:SPRITE_PALETTE)[(palette&15)*16];
}
