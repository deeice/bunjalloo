#include <algorithm>
#include <functional>
#include "libnds.h"
#include "ndspp.h"
#include "images.h"
#include "Arena.h"
#include "GfxData.h"
#include "Graphics.h"
#include "Text16.h"
#include "Wizard.h"

using namespace nds;
const unsigned short int  Graphics::s_chaosColours[] = {
  RGB5(31,31,31), // white
  RGB5(31,31,0), // yellow
  RGB5(0,31,31), //lightblue
  RGB5(0,31,0), //green
  RGB5(31,0,31), //purple
  RGB5(31,0,0), //red
  RGB5(0,0,31), //blue
  RGB5(20,20,20), //grey
  RGB5(20,20,0), // mustard
};
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
  Wizard * wizards(Wizard::players());
  std::for_each(wizards, wizards+8, std::mem_fun_ref(&Wizard::updateColour));
}

void Graphics::initialiseScreen()
{
  powerON(POWER_ALL_2D);
  Video & mainScreen = Video::instance();
  Video & subScreen  = Video::instance(1);
  subScreen.setMode(0);
  mainScreen.setMode(0);
  mainScreen.enableObjects();
  mainScreen.setFade(16);
  subScreen.setToTop();

  // initialise all palettes.
  loadAllPalettes();
  
  // load the sprite palette...
  ObjectPalette p(0);
  p.load(_binary_paletteb_pal_start, 128);

}

void Graphics::clearPalettes(void) {
  u16 i;
  for (i = 0; i < 16; i++) {
    Palette p0(0,i);
    Palette p1(1,i);
    p0.clear();
    p1.clear();
  }
}

void Graphics::loadSprite(const unsigned short * gfx, 
        const unsigned short * map)
{

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

void Graphics::animateSelection(int pal, nds::Color & c)
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

void Graphics::drawSplatFrame(int x, int y, int frame)
{
  Arena::instance().setPalette8(x*2,y*2,8);
  Arena::instance().drawGfx8(
      _binary_bolt_anim_raw_start, 
      _binary_bolt_anim_map_start, 
      x*2, y*2, frame);
}

