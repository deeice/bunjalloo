/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Canvas.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextField.h"
#include "TextListener.h"
#include "Palette.h"

const static nds::Color EDGE(20,20,20);
const static nds::Color SHADOW(28,28,28);

TextField::TextField(const UnicodeString & text) :
  TextContainer(text)
{ 
}

void TextField::paint(const nds::Rectangle & clip)
{
  TextContainer::paint(clip);
  // if it is a password field, and not empty, then set yellow to mask the password.
  if (not TextContainer::text().empty() and not echoText()) {
    nds::Canvas::instance().fillRectangle(m_bounds.x+1, m_bounds.y+1, m_bounds.w-1, m_bounds.h-2, nds::Color(29,29,10));
  }

  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w, EDGE);
  nds::Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h, EDGE);
  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w, SHADOW);
  nds::Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h, SHADOW);
}

bool TextField::touch(int x, int y)
{
  if (m_bounds.hit(x, y))
  {
    if (listener())
    {
      listener()->editText(this);
    }
    return true;
  }
  return false;
}
