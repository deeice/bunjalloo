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
#include "Button.h"
#include "ButtonListener.h"
#include "Canvas.h"
#include "Palette.h"
#include "Rectangle.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextContainer.h"
#include "WidgetColors.h"

using nds::Canvas;
using nds::Color;
using nds::Rectangle;
/*
const static Color BACKGROUND(27,27,27);
const static Color PRESSED(25,25,25);
const static Color EDGE(21,21,21);
const static Color SHINE(30,30,30);
*/

Button::Button() :
  TextContainer(),
  m_decoration(true)
{
  setBackgroundColor(WidgetColors::BUTTON_FOREGROUND);
}

Button::Button(const UnicodeString & label) :
  TextContainer(label),
  m_decoration(true)
{

}

void Button::paint(const nds::Rectangle & clip)
{
  // print the button
  unsigned short hilight(WidgetColors::BUTTON_SHINE);
  unsigned short lowlight(WidgetColors::BUTTON_SHADOW);

  if (m_decoration) {
    if (selected())
    {
      setBackgroundColor(WidgetColors::BUTTON_PRESSED);
      hilight = WidgetColors::BUTTON_SHADOW;
      lowlight = WidgetColors::BUTTON_SHINE;
    }
    else
    {
      setBackgroundColor(WidgetColors::BUTTON_FOREGROUND);
    }
  }
  TextContainer::paint(clip);

  if (m_decoration)
  {
    if (clip.hit(m_bounds.x, m_bounds.top()))
    {
      Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w, hilight);
    }
    Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h, hilight);
    if (clip.hit(m_bounds.x, m_bounds.bottom()))
    {
      Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w, lowlight);
    }
    Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h, lowlight);
  }
}

bool Button::touch(int x, int y)
{
  if (m_bounds.hit(x,y))
  {
    setSelected(true);
    if (listener())
    {
      listener()->pressed(this);
    }
    return true;
  } 
  setSelected(false);
  return false;
}


void Button::setDecoration(bool decorate)
{
  m_decoration = decorate;
}
