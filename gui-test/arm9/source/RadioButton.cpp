#include "libnds.h"
#include "RadioButton.h"
#include "Palette.h"
#include "radio_off.h"
#include "radio_on.h"

RadioButton::RadioButton():ToggleButton()
{
  setSize(16, 16);
}

const unsigned char * RadioButton::getGfxData() 
{
  if (selected())
  {
    return (const u8*)_binary_radio_on_img_bin_start;
  }
  else
  {
    // paint radio_off
    return (const u8*)_binary_radio_off_img_bin_start;
  }
}

const unsigned short * RadioButton::getPaletteData()
{
  if (selected())
  {
    return _binary_radio_on_pal_bin_start;
  }
  else
  {
    return _binary_radio_off_pal_bin_start;
  }
}


