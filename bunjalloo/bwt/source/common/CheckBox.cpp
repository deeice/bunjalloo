/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "libnds.h"
#include "CheckBox.h"
#include "Palette.h"
#include "checkbox.h"

CheckBox::CheckBox():ToggleButton()
{
  setSize(18, 18);
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


