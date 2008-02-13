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
#include "WidgetColors.h"
#include "Stylus.h"

TextField::TextField(const UnicodeString & text) :
  TextContainer(text), m_touched(false)
{
}

void TextField::paint(const nds::Rectangle & clip)
{
  TextContainer::paint(clip);
  // if it is a password field, and not empty, then set yellow to mask the password.
  if (not TextContainer::text().empty() and not echoText()) {
    nds::Canvas::instance().fillRectangle(m_bounds.x+1, m_bounds.y+1, m_bounds.w-1, m_bounds.h-2, nds::Color(29,29,10));
  }

  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w,
      m_touched?WidgetColors::SCROLLBAR_BACKGROUND_TOUCH:WidgetColors::BUTTON_SHADOW);
  nds::Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h,
      m_touched?WidgetColors::SCROLLBAR_BACKGROUND_TOUCH:WidgetColors::BUTTON_SHADOW);
  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w,
      m_touched?WidgetColors::SCROLLBAR_HANDLE_HELD:WidgetColors::BUTTON_FOREGROUND);
  nds::Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h,
      m_touched?WidgetColors::SCROLLBAR_HANDLE_HELD:WidgetColors::BUTTON_FOREGROUND);
  m_dirty = false;
}

bool TextField::stylusUp(const Stylus * stylus)
{
  bool consumed(false);
  if (m_touched and m_bounds.hit(stylus->lastX(), stylus->lastY()) and listener())
  {
    listener()->editText(this);
    m_dirty = true;
    consumed = true;
  }
  m_touched = false;
  return consumed;
}
bool TextField::stylusDownFirst(const Stylus * stylus)
{
  if (m_bounds.hit(stylus->startX(), stylus->startY()))
  {
    m_touched = true;
    m_dirty = true;
    return true;
  }
  return false;
}

bool TextField::stylusDownRepeat(const Stylus * stylus)
{
  return false;
}
bool TextField::stylusDown(const Stylus * stylus)
{
  if (not m_bounds.hit(stylus->lastX(), stylus->lastY()))
  {
    m_touched = false;
    m_dirty = true;
    return true;
  }
  return false;
}
