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
#include "Canvas.h"
#include "Palette.h"
#include "PasswordField.h"
#include "Rectangle.h"
#include "TextArea.h"
#include "HtmlElement.h"

using nds::Canvas;
using nds::Color;
using std::string;

PasswordField::PasswordField(HtmlElement * element, const TextArea * textArea)
  : InputText(element, textArea)
{ }

void PasswordField::draw(TextArea * textArea)
{
  FormControl::draw(textArea);
  UnicodeString text;
  UnicodeString value(m_element->attribute("value"));
  UnicodeString::const_iterator it(value.begin());
  int size(0);
  Font::Glyph g;
  textArea->font().glyph('*', g);
  for (; it != value.end() and size < m_size->w; ++it)
  {
    if (size + g.width < m_size->w) {
      size += g.width;
      text += *it;
    } else {
      break;
    }
  }
  UnicodeString pass(text.length(), '*');
  textArea->printu(pass);
}
