#include "libnds.h"
#include "CheckBox.h"
#include "Palette.h"
#include "checkbox.h"

CheckBox::CheckBox():ToggleButton()
{
  setSize(16, 16);
}

const unsigned char * CheckBox::getGfxData() 
{
  if (selected())
  {
    return (const u8*)&_binary_checkbox_img_bin_start[16*8];
  }
  else
  {
    return (const u8*)_binary_checkbox_img_bin_start;
  }
}

const unsigned short * CheckBox::getPaletteData()
{
  return _binary_checkbox_pal_bin_start;
}


