#include "libnds.h"
#include "SDLhandler.h"
#include "Palette.h"

using namespace nds;
/*
static unsigned short BG_PALETTE[256];
static unsigned short BG_PALETTE_SUB[256];
static unsigned short SPRITE_PALETTE[256];
static unsigned short SPRITE_PALETTE_SUB[256];
*/

Color::Color(unsigned short s):m_c(s)
{}
Color::Color(int r, 
    int g, 
    int b)
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

Palette::Palette(int screen) : 
  m_p(screen?
      SDLhandler::instance().subBackgroundPaletteMem() :
      SDLhandler::instance().backgroundPaletteMem()),
  m_16(false)
{ }

// careful - m_p is u16, but it should be u32
Palette::Palette(int screen, int palette):
  m_p(&(screen?
      SDLhandler::instance().subBackgroundPaletteMem() :
      SDLhandler::instance().backgroundPaletteMem())[(palette&15)*16]),
  m_16(true)
{ }

void Palette::load(const unsigned short * palette, unsigned int length)
{
  if (!length or length>256)
    length = m_16?16:256;
  for (unsigned int i = 0; i < length; ++i)
  {
    m_p[i] = palette[i];
  }
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
  m_p = screen? SDLhandler::instance().subSpritePaletteMem() : 
    SDLhandler::instance().spritePaletteMem();
}

ObjectPalette::ObjectPalette(int screen, int palette):Palette(screen, palette)
{
  m_p = &(screen? SDLhandler::instance().subSpritePaletteMem() : 
    SDLhandler::instance().spritePaletteMem())[(palette&15)*16];
}
