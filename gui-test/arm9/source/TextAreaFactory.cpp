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
#include "ndspp.h"
#include "libnds.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "Font.h"
#include "vera.h"

TextArea * TextAreaFactory::create()
{
  static Font * font(new Font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start));
  TextArea * t = new TextArea(font);
  t->setPalette((const char*)_binary_vera_pal_bin_start, 32);
  return t;
}
