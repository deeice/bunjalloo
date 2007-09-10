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
#include <algorithm>
#include "Canvas.h"
#include "EditableTextArea.h"
#include "Palette.h"
#include "ScrollPane.h"
#include "TextListener.h"
#include "UTF8.h"
#include "WidgetColors.h"
using nds::Canvas;
using nds::Color;

EditableTextArea::EditableTextArea(Font * font) :
  TextArea(font),
  m_caretLine(-1),
  m_caretChar(-1),
  m_caretPixelX(-1),
  m_appendedNewLine(false),
  m_echoText(true),
  m_scrollPane(0)
{
}

void EditableTextArea::printu(const UnicodeString & unicodeString)
{
  if (m_echoText)
  {
    TextArea::printu(unicodeString);
  }
  else
  {
    UnicodeString tmp(unicodeString.length(), '*');
    TextArea::printu(tmp);
  }
}

void EditableTextArea::paint(const nds::Rectangle & clip)
{
  TextArea::paint(clip);
  Canvas::instance().horizontalLine(clip.x, clip.top(), clip.w, WidgetColors::BUTTON_SHADOW);
  Canvas::instance().verticalLine(clip.left(), clip.top(), clip.h, WidgetColors::BUTTON_SHADOW);
  Canvas::instance().horizontalLine(clip.x, clip.bottom()-1, clip.w, WidgetColors::SCROLLBAR_ARROW);
  Canvas::instance().verticalLine(clip.right(), clip.top(), clip.h, WidgetColors::SCROLLBAR_ARROW);
  // now draw the caret too
  if (m_caretLine != -1)
  {
    // see if line is visible
    int caretLinePos(m_bounds.top() + m_caretLine*font().height());
    if (caretLinePos <= m_bounds.bottom())
    {
      // draw
      if (m_caretPixelX == -1)
      {
        m_caretPixelX = 0;
        UnicodeString tmp;
        if (not echoText())
        {
          UnicodeString(m_document[m_caretLine].length(),'*').swap(tmp);
        }
        const UnicodeString & line(echoText()?m_document[m_caretLine]:tmp);
        for (int i = 0; i < (int)line.length() and i < m_caretChar; ++i)
        {
          unsigned int value(line[i]);
          if (value == UTF8::MALFORMED)
            value = '?';
          Font::Glyph g;
          font().glyph(value, g);
          m_caretPixelX += g.width;
        }
      }
      Canvas::instance().verticalLine(m_caretPixelX, caretLinePos, font().height(), Color(31,0,0));
    }

  }
}

void EditableTextArea::text(UnicodeString & returnString) const
{
  document(returnString);
}

void EditableTextArea::deleteChar()
{
  // delete a single char
  if (m_caretLine == -1)
  {
    UnicodeString & line(currentLine());
    if (not line.empty())
    {
      line.erase(line.length()-1, 1);
    }
  }
  else
  {
    if (m_caretChar == 0) {
      if (m_caretLine) {
        m_caretLine--;
        m_caretChar = m_document[m_caretLine].length();
      }
      else
      {
        return;
      }
    }

    UnicodeString & line(m_document[m_caretLine]);
    m_caretChar -= 1;
    if (m_caretChar == -1)
    {
      m_document.erase(m_document.begin()+m_caretLine);
    }
    else
    {
      line.erase(m_caretChar, 1);
    }
    recalculateCaret();
  }
  resizeParent();
}

void EditableTextArea::recalculateCaret()
{
  UnicodeString & line(m_document[m_caretLine]);
  m_caretPixelX = -1;
  int oldLength = line.length();
  int oldPrevLength = -1;
  if (m_caretLine > 0)
  {
    oldPrevLength = m_document[m_caretLine-1].length();
  }
  // redo the text layout
  layoutText();
  if (m_appendedNewLine)
  {
    m_caretChar = 0;
    m_caretLine++;
    m_appendedNewLine = false;
    return;
  }
  int newLength = m_document[m_caretLine].length();
  // work out if the caret should overflow
  // also, what if it "underflows"?
  if (newLength < oldLength)
  {
    // word has wrapped to next line. To calculate where we are, work out the
    // difference from previous to now
    // Also, if we add a space to separate a word, the word may go to the *previous* line.
    int diff = oldLength - newLength;
    if (m_caretChar >= newLength)
    {
      m_caretLine++;
      // fix Issue 19 - off by one on wrap around new line.
      m_caretChar = diff;
      m_caretPixelX = -1;
    }
  }
  // check for underflow condition
  int newPrevLength = -1;
  if (m_caretLine > 0)
    newPrevLength = m_document[m_caretLine-1].length();
  if (oldPrevLength != -1 and newPrevLength != -1 and oldPrevLength < newPrevLength)
  {
    // the previous line has eaten some of our text
    m_caretChar = 0;
    m_caretPixelX = -1;
  }
  // ensure m_document is never empty.
  currentLine();
}
void EditableTextArea::appendText(const UnicodeString & unicodeString)
{
  if (m_caretLine == -1)
  {
    TextArea::appendText(unicodeString);
    // select the end of the text
    if (not listener())
    {
      m_caretLine = m_document.size()-1;
      m_caretChar = m_document[m_caretLine].size();
      m_caretPixelX = -1;
    }
  }
  else
  {
    // caret is on a line.
    // what if the line goes over the edge?
    // the m_caretLine should increase by one and reshuffle the line.
    UnicodeString & line(m_document[m_caretLine]);
    // Check we don't walk off the edge, just in case...
    if (m_caretChar > (int)line.length())
    {
      m_caretChar = line.length();
    }
    line.insert(m_caretChar, unicodeString);
    if (unicodeString.length() == 1 and unicodeString[0] == '\n')
    {
      m_appendedNewLine = true;
    } 
    else 
    {
      m_appendedNewLine = false;
      m_caretChar++;
    }
    recalculateCaret();
  }
  resizeParent();
}

void EditableTextArea::clearText()
{
  TextArea::clearText();
  layoutText();
  resizeParent();
  m_caretLine = -1;
  m_caretChar = -1;
  m_caretPixelX = -1;
  m_appendedNewLine = false;
}

void EditableTextArea::resizeParent()
{
  if (m_scrollPane)
  {
    int height = std::max(preferredSize().h, m_scrollPane->height());
    setSize(width(), height);
    m_preferredHeight = height;
    m_scrollPane->setSize(m_scrollPane->width(), m_scrollPane->height());
    m_scrollPane->scrollToPercent(255);
  }
}

void EditableTextArea::setCaret(int x, int y)
{
  // set the caret position based on x, y position
  // first see if in bounds
  if (m_bounds.hit(x, y))
  {
    // next see if the caret can be set.
    m_caretLine = (y - m_bounds.y)/font().height();
    if (m_caretLine >= (int)m_document.size())
    {
      m_caretLine = -1;
      return;
    }
    m_caretChar = 0;
    m_caretPixelX = -1;
    UnicodeString tmp;
    if (not echoText())
    {
      UnicodeString(m_document[m_caretLine].length(),'*').swap(tmp);
    }
    const UnicodeString & line(echoText()?m_document[m_caretLine]:tmp);
    int size = 0;
    for (int i = 0; i < (int)line.length(); ++i)
    {
      unsigned int value(line[i]);
      if (value == UTF8::MALFORMED)
        value = '?';
      Font::Glyph g;
      font().glyph(value, g);
      size += g.width;
      if (size > x)
      {
        size -= g.width;
        m_caretChar = i;
        m_caretPixelX = size;
        return;
      }
    }
    m_caretChar = line.length();
    m_caretPixelX = size;
  }
}


void EditableTextArea::setText(const UnicodeString & text)
{
  clearText();
  appendText(text);
  resizeParent();
}

bool EditableTextArea::touch(int x, int y)
{
  // if inside a scrollPane, check relative to the bounds of parent
  // otherwise check our bounds.
  if ( (m_scrollPane and m_scrollPane->bounds().hit(x, y) and m_bounds.hit(x, y) ) 
      or (not m_scrollPane and m_bounds.hit(x, y)))
  {
    if (listener())
    {
      listener()->editText(this);
    }
    else
    {
      setCaret(x, y);
    }
    return true;
  }
  return false;
}

int EditableTextArea::caretLine() const
{
  return m_caretLine;
}

bool EditableTextArea::isMultiLine() const
{
  return true;
}

void EditableTextArea::setParentScroller(ScrollPane * scrollPane)
{
  m_scrollPane = scrollPane;
}

bool EditableTextArea::caretVisible() const
{
  if (m_caretLine != -1)
  {
    int caretLinePos(m_bounds.top() + m_caretLine*font().height());
    if (caretLinePos <= m_bounds.bottom() 
        and m_scrollPane
        and caretLinePos <= m_scrollPane->bounds().bottom() 
        and caretLinePos > m_scrollPane->bounds().top())
    {
      return true;
    }
  }
  return false;
}
