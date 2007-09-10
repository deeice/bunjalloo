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
#include "EditableTextArea.h"
#include "RichTextArea.h"
// #include "vera.h"

Font * TextAreaFactory::s_font(0);
const char* TextAreaFactory::s_paletteData(0);
int TextAreaFactory::s_paletteSize(0);
std::string TextAreaFactory::s_name;

TextArea * TextAreaFactory::create(TextType type)
{
  // static Font font((unsigned char*)_binary_vera_img_bin_start, (unsigned char*)_binary_vera_map_bin_start);

  TextArea * t(0);
  switch (type)
  {
    case TXT_NORMAL:
      t = new TextArea(s_font);
      break;
    case TXT_EDIT:
      t = new EditableTextArea(s_font);
      break;
    case TXT_RICH:
      t = new RichTextArea(s_font);
      break;
  }
  if (s_name.empty())
  {
    t->setPalette(s_paletteData, s_paletteSize);
  }
  else
  {
    t->setPalette(s_name);
  }
  return t;
}

void TextAreaFactory::setFont(Font * font)
{
  s_font = font;
}

void TextAreaFactory::usePaletteName(const std::string & name)
{
  s_name = name;
  s_paletteData = 0;
}
void TextAreaFactory::usePaletteData(const char * data, int size)
{
  s_name.clear();
  s_paletteData = data;
  s_paletteSize = size;
}