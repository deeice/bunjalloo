#include <nds.h>
#include "ndspp.h"
#include "Text16.h"
#include "Graphics.h"
#include "GfxData.h"

using namespace nds;
void Graphics::loadPalette(int from, int to)
{
  Palette p(0,to);
  p.load(s_paletteData[from].data, s_paletteData[from].size);
}
void Graphics::loadAllPalettes()
{
  // load palettes to memory
  for (int i = 0; i < 10; ++i)
  {
    Graphics::loadPalette(i,i);
  }
}

void Graphics::initialiseScreen()
{
  powerON(POWER_ALL_2D);
  Video & mainScreen = Video::instance();
  Video & subScreen  = Video::instance(1);
  subScreen.mode(0);
  mainScreen.mode(0);
  mainScreen.setWhite(16);
  subScreen.setToTop();

  // initialise all palettes.
  loadAllPalettes();
  nds::Video::instance(0).whiteout(false, 1);
}

void Graphics::clearPalettes(void) {
  u16 i;
  for (i = 0; i < 16; i++) {
    Palette p(0,i);
    p.clear();
  }
}

Graphics::Graphics():
m_colour(31),m_gradient(1)
{
}

Graphics & Graphics::instance()
{
  static Graphics s_instance;
  return s_instance;
}

void Graphics::setAnimationParams(signed char c, signed char g)
{
  m_colour = c;
  m_gradient = g;
}
void Graphics::setAnimationColour(signed char c)
{
  m_colour = c;
}

void Graphics::animateSelection(int pal, Color & c)
{
  s8 r = c.red();
  s8 g = c.green();
  s8 b = c.blue();
  
  r = r + m_colour;
  g = g + m_colour;
  b = b + m_colour;
  
  r = r>31?31:r;
  g = g>31?31:g;
  b = b>31?31:b;
  
  r = r<0?0:r;
  g = g<0?0:g;
  b = b<0?0:b;
  
  c.set(r, g, b);
  
  Text16::instance().setColour(pal, c);
  
  m_colour += m_gradient;
  
  if (m_colour >= 31) {
    m_gradient = -8;
    m_colour = 31;
  } else if (m_colour <= -15) {
    m_gradient = 8;
    m_colour = -15;
  }
  
}

