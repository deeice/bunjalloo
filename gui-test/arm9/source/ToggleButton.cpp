#include "ToggleButton.h"
#include "ButtonListener.h"
#include "Canvas.h"

ToggleButton::ToggleButton():
  Component()
{}

void ToggleButton::paint(const nds::Rectangle & clip)
{
  // paint radio_on or radio_off 
  const unsigned char * gfx = getGfxData();
  const unsigned short * palette = getPaletteData();

  nds::Canvas & canvas(nds::Canvas::instance());

  // paint radio_on - a 16x16 graphic in sprite tile format
  for (int tile = 0; tile < 4; tile++)
  {
    int yi = m_bounds.y + ((tile/2)*8);
    int xi = m_bounds.x + ((tile&1)*8);
    for (int y = yi; y < yi+8; ++y)
    {
      for (int x = xi; x < xi+8; x++)
      {
        canvas.drawPixel(x, y, palette[*gfx++]);
      }
    }
  }
}

bool ToggleButton::touch(int x, int y)
{
  if (m_bounds.hit(x, y))
  {
    if (listener())
    {
      listener()->pressed(this);
    }
    else
    {
      setSelected(not selected());
    }
    return true;
  }
  return false;
}

