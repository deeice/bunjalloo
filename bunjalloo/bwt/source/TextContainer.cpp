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
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextContainer.h"

const int TextContainer::BORDER_WIDTH(5);
const int TextContainer::BORDER_HEIGHT(5);

TextContainer::TextContainer(const UnicodeString & text) :
  Component()
{
  add(TextAreaFactory::create());

  textArea()->setSize(textArea()->textSize(text), textArea()->font().height());
  nds::Rectangle textSize = textArea()->preferredSize();
  m_preferredWidth = textSize.w+BORDER_WIDTH;
  m_preferredHeight = textSize.h+BORDER_HEIGHT;
  setText(text);
}


TextContainer::TextContainer():
  Component()
{
  add(TextAreaFactory::create());
}

void TextContainer::setText(const UnicodeString & text)
{
  bool needLayout = false;
  if (m_text != text)
  {
    needLayout = true;
  }
  m_text = text;
  if (needLayout)
    layout();
}

void TextContainer::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w, h);

  if ((int)w < (m_preferredWidth+BORDER_WIDTH))
  {
    m_preferredWidth = w+BORDER_WIDTH;
  }
  else
  {
    m_preferredWidth = w;
  }
  textArea()->setSize(w, h);
  m_preferredHeight = textArea()->font().height()+BORDER_HEIGHT;
  /*
  int tmpPW = textArea()->preferredSize().w+BORDER_WIDTH;
  if (tmpPW > m_preferredWidth)
    m_preferredWidth = tmpPW;
  */
  layout();
}

void TextContainer::layout()
{
  if (textArea()->width() == 0)
  {
    textArea()->setSize(textArea()->textSize(m_text), textArea()->font().height());
  }
  UnicodeString appendText;
  UnicodeString::const_iterator it(m_text.begin());
  int size(0);
  for (; it != m_text.end() and size <= textArea()->width(); ++it)
  {
    Font::Glyph g;
    textArea()->font().glyph(*it, g);
    if ((size + g.width) <= textArea()->width()) {
      size += g.width;
      appendText += *it;
    } else {
      break;
    }
  }
  textArea()->clearText();
  textArea()->appendText(appendText);
  if (width() == 0)
  {
    m_bounds.w = size + BORDER_WIDTH;
    m_preferredWidth = m_bounds.w;
    m_bounds.h = textArea()->font().height() + BORDER_HEIGHT;
    m_preferredHeight = m_bounds.h;
  }
}

void TextContainer::paint(const nds::Rectangle & clip)
{
  textArea()->setLocation(m_bounds.x+BORDER_WIDTH/2, m_bounds.y+BORDER_HEIGHT/2);
  textArea()->paint(clip);
}

void TextContainer::setBackgroundColor(unsigned short color)
{
  textArea()->setBackgroundColor(color);
}

const UnicodeString & TextContainer::text() const
{
  return m_text;
}

