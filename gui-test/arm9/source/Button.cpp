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
const static Color BACKGROUND(26,26,26);
const static Color PRESSED(23,23,23);
const static Color EDGE(21,21,21);
const static Color SHINE(30,30,30);
static const int BORDER_WIDTH(5);
static const int BORDER_HEIGHT(5);

Button::Button() : Component(), m_pressed(false), m_label(TextAreaFactory::create())
{
  m_label->setBackgroundColor(BACKGROUND);
}

Button::Button(const UnicodeString & label) :
  Component(),
  m_pressed(false),
  m_label(TextAreaFactory::create())
{
  setLabel(label);
  /*
  int textSize = m_label->textSize(label);
  m_label->setSize(textSize+BORDER_WIDTH, m_label->font().height()+BORDER_HEIGHT);
  m_label->appendText(label);
  m_preferredWidth = textSize+BORDER_WIDTH;
  m_preferredHeight = m_label->preferredSize().h+BORDER_HEIGHT;
  m_label->setBackgroundColor(BACKGROUND);
  */
}
Button::~Button()
{
  delete m_label;
}

void Button::setSize(unsigned int w, unsigned int h)
{
  m_label->setSize(w, h);
  if ((int)w != m_preferredWidth)
  {
    // need to layout the text area again.
    m_preferredWidth = w;
  }
  Component::setSize(w, h);
  m_preferredHeight = m_label->preferredSize().h+BORDER_HEIGHT;
}

void Button::paint(const nds::Rectangle & clip)
{
  // print the button
  unsigned short hilight(SHINE);
  unsigned short lowlight(EDGE);

  if (m_pressed)
  {
    m_label->setBackgroundColor(PRESSED);
    hilight = EDGE;
    lowlight = SHINE;
  }
  else
  {
    m_label->setBackgroundColor(BACKGROUND);
  }
  m_label->setLocation(m_bounds.x+BORDER_WIDTH/2, m_bounds.y+BORDER_HEIGHT/2);
  m_label->paint(clip);
  Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w, hilight);
  Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h, hilight);
  Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w, lowlight);
  Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h, lowlight);
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

void Button::setLabel(const UnicodeString & label)
{
  if (width() == 0)
  {
    m_label->clearText();
    int textSize = m_label->textSize(label);
    m_label->setSize(textSize+BORDER_WIDTH, m_label->font().height()+BORDER_HEIGHT);
    m_label->appendText(label);
    m_preferredWidth = textSize+BORDER_WIDTH;
    m_preferredHeight = m_label->preferredSize().h+BORDER_HEIGHT;
    m_label->setBackgroundColor(BACKGROUND);
  }
  else
  {

    UnicodeString appendText;
    appendText.clear();
    UnicodeString::const_iterator it(label.begin());
    int size(0);
    for (; it != label.end() and size < width(); ++it)
    {
      Font::Glyph g;
      m_label->font().glyph(*it, g);
      if ((size + g.width) < m_bounds.w) {
        size += g.width;
        appendText += *it;
      } else {
        break;
      }
    }
    m_label->clearText();
    m_label->appendText(appendText);
    m_label->setBackgroundColor(BACKGROUND);
  }
}
const UnicodeString & Button::label() const
{
  return m_label->text();
}
