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

using nds::Canvas;
using nds::Color;
using nds::Rectangle;
const static Color BACKGROUND(26,26,26);
const static Color PRESSED(23,23,23);
const static Color EDGE(21,21,21);
const static Color SHINE(30,30,30);

Button::Button() :
  TextContainer(),
  m_pressed(false),
  m_decoration(true),
  m_listener(0)
{
  setBackgroundColor(BACKGROUND);
}

Button::Button(const UnicodeString & label) :
  TextContainer(label),
  m_pressed(false),
  m_decoration(true),
  m_listener(0)
{

}

void Button::paint(const nds::Rectangle & clip)
{
  // print the button
  unsigned short hilight(SHINE);
  unsigned short lowlight(EDGE);

  if (m_decoration) {
    if (m_pressed)
    {
      setBackgroundColor(PRESSED);
      hilight = EDGE;
      lowlight = SHINE;
    }
    else
    {
      setBackgroundColor(BACKGROUND);
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
  m_pressed = false;
  if (m_bounds.hit(x,y))
  {
    m_pressed = true;
    if (m_listener)
    {
      m_listener->pressed(this);
    }
  } 
  return m_pressed;
}

void Button::setPressed(bool pressed)
{
  m_pressed = pressed;
}

void Button::setListener(ButtonListener * listener)
{
  m_listener = listener;
}

void Button::removeListener(ButtonListener * listener)
{
  m_listener = 0;
}

void Button::setDecoration(bool decorate)
{
  m_decoration = decorate;
}
