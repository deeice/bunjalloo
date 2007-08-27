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
#include "TextAreaFactory.h"
#include "TextArea.h"

using nds::Canvas;
using nds::Color;
using nds::Rectangle;
const static Color BACKGROUND(26,26,30);
const static Color PRESSED(16,16,20);
const static Color EDGE(21,21,21);

Button::Button() : Component(), m_pressed(false), m_label(TextAreaFactory::create())
{
  m_label->setBackgroundColor(BACKGROUND);
}

Button::Button(const UnicodeString & label) :
  Component(),
  m_pressed(false),
  m_label(TextAreaFactory::create())
{
  int textSize = m_label->textSize(label);
  m_label->setSize(textSize+5, m_label->font().height()+5);
  m_label->appendText(label);
  m_preferredWidth = textSize+5;
  m_preferredHeight = m_label->preferredSize().h+5;
  m_label->setBackgroundColor(BACKGROUND);
}

void Button::setSize(unsigned int w, unsigned int h)
{
  if (m_bounds.w != (int)w and m_bounds.h != (int)h)
  {
    Component::setSize(w, h);
    m_label->setSize(w, h);
    m_preferredHeight = m_label->preferredSize().h;
  }
  else
  {
    Component::setSize(w, h);
    m_label->setSize(w, h);
    m_preferredHeight = m_label->preferredSize().h;
  }
}

void Button::setText(const UnicodeString & label)
{
  m_label->appendText(label);
}

void Button::paint(const nds::Rectangle & clip)
{
  // print the button
  if (m_pressed) {
    m_label->setBackgroundColor(PRESSED);
  }
  else {
    m_label->setBackgroundColor(BACKGROUND);
  }
  m_label->setLocation(m_bounds.x+2, m_bounds.y);
  m_label->paint(clip);
  Canvas::instance().drawRectangle(m_bounds.x, m_bounds.y, m_bounds.w-1, m_bounds.h-1, EDGE);
}

bool Button::touch(int x, int y)
{
  m_pressed = false;
  if (m_bounds.hit(x,y))
  {
    m_pressed = true;
  } 
  return m_pressed;
}
