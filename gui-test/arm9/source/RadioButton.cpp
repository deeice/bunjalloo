#include "libnds.h"
#include "RadioButton.h"
#include "Palette.h"
#include "radiobutton.h"

RadioButton::RadioButton():ToggleButton()
{
  setSize(16, 16);
}

const unsigned char * RadioButton::getGfxData() 
{
  if (selected())
  {
    return (const u8*)&_binary_radiobutton_img_bin_start[16*8];
  }
  else
  {
    // paint radio_off
    return (const u8*)_binary_radiobutton_img_bin_start;
  }
}

const unsigned short * RadioButton::getPaletteData()
{
  return _binary_radiobutton_pal_bin_start;
}


