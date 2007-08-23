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
#include "Button.h"
#include "Rectangle.h"
#include "TextArea.h"
#include "HtmlElement.h"

using nds::Canvas;
using nds::Color;
using nds::Rectangle;

Button::Button(HtmlElement * element, const TextArea * textArea)
  : FormControl(element)
{
  int size = textArea->textSize(m_element->attribute("value"));
  if ( size > m_size->w)
  {
    if (size > MAX_SIZE)
      size = MAX_SIZE;
    m_size->w = size;
  }
}

void Button::draw(TextArea * textArea)
{
  FormControl::draw(textArea);
  Canvas::instance().fillRectangle(m_size->x+1, m_size->y+1, m_size->w-1, m_size->h-1, Color(26,26,30));
  // print the button
  textArea->setBackgroundColor(Color(26,26,30));
  textArea->printu(m_element->attribute("value"));
  textArea->setBackgroundColor(Color(31,31,31));
}

