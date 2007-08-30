#include "libnds.h"
#include "CheckBox.h"
#include "Palette.h"
#include "checkbox_off.h"
#include "checkbox_on.h"

CheckBox::CheckBox():ToggleButton()
{
  setSize(16, 16);
}

const unsigned char * CheckBox::getGfxData() 
{
  if (selected())
  {
    return (const u8*)_binary_checkbox_on_img_bin_start;
  }
  else
  {
    // paint checkbox_off
    return (const u8*)_binary_checkbox_off_img_bin_start;
  }
}

const unsigned short * CheckBox::getPaletteData()
{
  if (selected())
  {
    return _binary_checkbox_on_pal_bin_start;
  }
  else
  {
    return _binary_checkbox_off_pal_bin_start;
  }
}


