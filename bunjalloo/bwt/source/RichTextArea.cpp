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
#include "LinkListener.h"
#include "Palette.h"
#include "RichTextArea.h"
#include "UTF8.h"
#include "Rectangle.h"
#include "Canvas.h"
#include "WidgetColors.h"
using namespace std;
using namespace nds;

RichTextArea::RichTextArea(Font * font) :
  TextArea(font),
  m_documentSize(0),
  m_state(Link::STATE_PLAIN),
  m_linkListener(0),
  m_centred(false),
  m_outlined(false)
{
}

RichTextArea::~RichTextArea()
{
  removeClickables();
}

static void deleteLink(Link * link)
{
  delete link;
}

void RichTextArea::removeClickables()
{
  for_each(m_links.begin(), m_links.end(), deleteLink);
  m_links.clear();
  m_state = Link::STATE_PLAIN;
}

static bool isEmpty(const UnicodeString & line)
{
  UnicodeString::const_iterator it(line.begin());
  for (; it != line.end(); ++it)
  {
    if (not isWhitespace(*it))
    {
      return false;
    }
  }
  return true;
}

void RichTextArea::appendText(const UnicodeString & unicodeString)
{
  if (m_document.size() > 1 and isEmpty(unicodeString) and isEmpty(currentLine()) and isEmpty(m_document[m_document.size()-2]))
  {
    // avoid duplicate empty lines
    return;
  }
  TextArea::appendText(unicodeString);
  m_documentSize = documentSize();
}

void RichTextArea::add(Component * child)
{
  Component::add(child);

  Rectangle bounds(child->preferredSize());
  child->setSize(bounds.w, bounds.h);
  int w = bounds.w;
  if (m_bounds.w < bounds.w) {
    w = m_bounds.w;
  }
  child->setSize(w, bounds.h);

  int x = m_appendPosition;
  if ((x + child->width()) > width())
  {
    // out of bounds
    m_appendPosition = child->width();
    m_document.push_back(UnicodeString());
    m_preferredHeight += lineHasComponent(m_document.size()-1)? m_lineHeight[m_document.size()-1]:font().height();
    x = 0;
  }
  else
  {
    m_appendPosition += child->width();
  }

  int y = 0;
  for (unsigned int i = 0; i < m_document.size()-1; ++i)
  {
    if (lineHasComponent(i))
      y += m_lineHeight[i];
    else
      y += font().height();
  }

  child->setLocation(x, y);

  m_lineHeight[m_document.size()-1] = std::max(child->height(), font().height());
  m_childPositions.push_back(m_documentSize);
  m_preferredHeight += m_lineHeight[m_document.size()-1] - font().height();
  // problems here -
  // setSize should update the child sizes and text positions
  // setLocation should update the child locations
  // on paint, should constantly look for next child component while printing text
  // upon finding the child component, increment the cursorx, cursory to account for it.
}

void RichTextArea::setLocation(unsigned int x, unsigned int y)
{
  // work out dx, dy
  int dx = this->x() - x;
  int dy = this->y() - y;
  Component::setLocation(x, y);
  if (dx == 0 and dy == 0)
    return;

  for (std::vector<Component*>::iterator it(m_children.begin());
      it != m_children.end();
      ++it)
  {
    // move children dx, dy
    Component * child(*it);
    child->setLocation( child->x() - dx, child->y() - dy);
  }
}

void RichTextArea::addLink(const std::string & href, bool viewed)
{
  Link * link = new Link(href);
  link->setTextStart(m_documentSize);
  m_links.push_back(link);
  m_state = Link::STATE_LINK;
  if (viewed)
  {
    link->setColor(WidgetColors::LINK_CLICKED);
  }
}

void RichTextArea::endLink()
{
  if (m_state == Link::STATE_LINK)
  {
    m_state = Link::STATE_PLAIN;
    Link & link(*m_links.back());
    link.setTextEnd(m_documentSize);
    if (link.textEnd() == link.textStart())
    {
      m_links.pop_back();
      delete &link;
    }
    else
    {
      int lastChar = (int) ( currentLine()[currentLine().length()-1] );
      if (isWhitespace(lastChar))
      {
        link.setTextEnd(m_documentSize-1);
      }
    }
  }
}

void RichTextArea::addImage(const std::string & src)
{
  if (m_state == Link::STATE_LINK)
  {
    // adjust the last link to be this color
    // e.g. an image that is a link. => green text [like this] but also a link.
    string href = m_links.back()->href();
    endLink();
    addLink(href);
    m_links.back()->setColor(WidgetColors::LINK_IMAGE);
  }
  else
  {
    Link * link = new Link(WidgetColors::LINK_IMAGE);
    link->setTextStart(m_documentSize);
    m_links.push_back(link);
    m_state = Link::STATE_COLOR;
  }
  m_links.back()->setSrc(src);
}

void RichTextArea::endImage()
{
  if (m_state == Link::STATE_COLOR)
  {
    m_state = Link::STATE_PLAIN;
    Link & link(*m_links.back());
    link.setTextEnd(m_documentSize);
    if (link.textEnd() == link.textStart())
    {
    //printf("pop link endCol\n");
      m_links.pop_back();
      delete &link;
    }
  }
  else if (m_state == Link::STATE_LINK)
  {
    string href = m_links.back()->href();
    endLink();
    addLink(href);
  }
}

void RichTextArea::incrLine()
{
  LineHeightMap::const_iterator it(m_lineHeight.find(m_lineNumber));
  if (m_lineHeight.end() != it)
  {
    m_cursory += it->second;
  }
  else
  {
    m_cursory += font().height();
  }
  m_cursorx = m_initialCursorx;
  m_lineNumber++;
}

bool RichTextArea::lineHasComponent(int line) const
{
  LineHeightMap::const_iterator it(m_lineHeight.find(line));
  return (m_lineHeight.end() != it);
}

void RichTextArea::printu(const UnicodeString & unicodeString)
{
  // print at the start of the line this line's text
  // but adding components means that the text may be interspersed between components
  // i.e. any combination of
  // text [   ]  text [  ] text [ ] text

  // take care - can have text first or component first
  UnicodeString::const_iterator it(unicodeString.begin());
  static const UnicodeString delimeter(string2unicode(" \r\n	"));
  unsigned int lastPosition = unicodeString.find_last_not_of(delimeter);
  unsigned int i = 0;
  bool hasComponent(lineHasComponent(m_lineNumber));
  bool centred(m_centred);
  // can't centre the line if there is a component on it
  if (hasComponent)
  {
    centred = false;
  }
  if (centred)
  {
    // find size of line, change m_cursorx accordingly
    int w = textSize(unicodeString.substr(0, lastPosition));
    m_cursorx = (m_bounds.w - w)/2;
  }

  /*
  printf("hasComponent %s , m_currentChildIndex %d string %d\n", hasComponent?"Yes":"no", m_currentChildIndex,
      unicodeString.size());
  */
  // this is for the case of components alone on a line, still need to jump past them
  // so that the next component is considered (otherwise we miss them)
  if (unicodeString.size() == 0 and hasComponent)
  {
    // how to factor this loop?
    while (hasComponent and m_currentChildIndex < m_childPositions.size()
        and m_paintPosition == m_childPositions[m_currentChildIndex])
    {
      Component * c(m_children[m_currentChildIndex]);
      if (m_cursory == c->y())
      {
        m_cursorx += c->width();
      }
      else
      {
        break;
      }
      ++m_currentChildIndex;
    }
  }

  for (; it != unicodeString.end() ; ++it, ++i)
  {
    // check for components
    while (hasComponent and m_currentChildIndex < m_childPositions.size()
        and m_paintPosition == m_childPositions[m_currentChildIndex])
    {
      Component * c(m_children[m_currentChildIndex]);
      if (m_cursory == c->y())
      {
        m_cursorx += c->width();
      }
      ++m_currentChildIndex;
    }
    if (m_nextEvent == m_paintPosition)
    {
      handleNextEvent();
    }
    ++m_paintPosition;
    if (i > lastPosition) {
      continue;
    }
    unsigned int value(*it);
    if ( doSingleChar(value) )
    {
      break;
    }
  }
}

void RichTextArea::handleNextEvent()
{
  switch (m_nextEventType)
  {
    case Link::STATE_LINK:
      if ( (*m_currentLink)->clicked() )
      {
        setTextColor(WidgetColors::LINK_CLICKED);
      }
      else
      {
        setTextColor((*m_currentLink)->color());
      }
      m_nextEvent = (*m_currentLink)->textEnd();
      (*m_currentLink)->setClicked(false);
      m_nextEventType = Link::STATE_PLAIN;
      setUnderline();
      break;
    case Link::STATE_PLAIN:
      setTextColor(0);
      setUnderline(false);
      if (m_currentLink != m_links.end())
        ++m_currentLink;
      if (m_currentLink != m_links.end())
      {
        m_nextEvent = (*m_currentLink)->textStart();
        m_nextEventType = (*m_currentLink)->eventType();
        if (m_nextEvent == m_paintPosition)
        {
          handleNextEvent();
        }
      } else {
        m_nextEvent = m_documentSize;
        m_nextEventType = Link::STATE_PLAIN;
      }
      break;
    case Link::STATE_COLOR:
      setTextColor((*m_currentLink)->color());
      m_nextEvent = (*m_currentLink)->textEnd();
      (*m_currentLink)->setClicked(false);
      m_nextEventType = Link::STATE_PLAIN;
      setUnderline(false);
      break;
  }
}

unsigned int RichTextArea::documentSize(int endLine, unsigned int * childIndex) const
{
  unsigned int total = 0;
  if (endLine == -1)
  {
    endLine = m_document.size();
  }
  unsigned int currentChildIndex = 0;
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  for (int i = 0; it != m_document.end() and i != endLine; ++it, ++i)
  {
    total += it->length();

    // skip to the next child position
    while (currentChildIndex < m_childPositions.size() and total > m_childPositions[currentChildIndex])
    {
      ++currentChildIndex;
    }
  }
  if (childIndex)
  {
    *childIndex = currentChildIndex;
  }
  return total;
}

int RichTextArea::linesToSkip() const
{
  if (m_bounds.y < 0)
  {
    return lineAt(0) + 1;
  }
  return 0;
}

void RichTextArea::paint(const nds::Rectangle & clip)
{
  // ensure we switch off links
  setTextColor(0);
  setUnderline(false);
  // see where the first link is
  m_nextEvent = 0;
  if (not m_links.empty())
  {
    m_currentLink = m_links.begin();
    m_nextEvent = (*m_currentLink)->textStart();
    m_nextEventType = (*m_currentLink)->eventType();
  }
  else
  {
    m_currentLink = m_links.end();
    m_nextEvent = m_documentSize;
    m_nextEventType = Link::STATE_PLAIN;
  }
  nds::Canvas::instance().setClip(clip);
  m_paintPosition = 0;
  m_lineNumber = 0;
  // work out what happens when we skip lines.
  // how to factor this ? copied from lineAt()
  int dy = (- m_bounds.y);
  int lineNum = 0;
  for (; dy > font().height();)
  {
    LineHeightMap::const_iterator it(m_lineHeight.find(lineNum));

    if (m_lineHeight.end() != it)
    {
      dy -= it->second;
    }
    else
    {
      dy -= font().height();
    }
    lineNum++;
  }
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  int skipLines(lineNum);
  m_currentChildIndex = 0;
  // printf("Skip %d lines dy %d\n", skipLines, dy);
  if (skipLines > 0)
  {
    checkSkippedLines(skipLines);
    m_lineNumber = skipLines;
    setCursor(m_bounds.x, -dy);
    it += skipLines;
  }
  else
  {
    setCursor(m_bounds.x, m_bounds.y);
  }

  //TextArea::paint(clip);
  if (m_outlined)
  {
    nds::Canvas::instance().
      drawRectangle(
            m_bounds.x,
            m_bounds.y,
            m_bounds.w-1,
            m_bounds.h,
            WidgetColors::BUTTON_SHADOW);
  }

  for (; it != m_document.end() and (m_cursory < m_bounds.bottom()) and (m_cursory < clip.bottom()); ++it)
  {
    printu(*it);
    incrLine();
  }
  for (std::vector<Component*>::iterator it(m_children.begin());
      it != m_children.end();
      ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    Rectangle thisClip(clip.intersect(bounds));
    if (thisClip.w == 0 and thisClip.h == 0)
      continue;
    c->paint(thisClip);
    nds::Canvas::instance().setClip(clip);
  }

}

void RichTextArea::checkSkippedLines(int skipLines)
{
  // the lines are skipped - see which link applies to us, if any
  unsigned int total = documentSize(skipLines, &m_currentChildIndex);

  // skipped "total" characters.
  m_paintPosition = total;
  // see which link applies
  bool handled(false);
  LinkList::const_iterator linkIt(m_links.begin());
  for (; linkIt != m_links.end(); ++linkIt)
  {
    Link * l(*linkIt);
    // if we are in the middle of a link:
    if (m_paintPosition >= l->textStart() and m_paintPosition <= l->textEnd())
    {
      m_nextEventType = l->eventType();
      m_currentLink = linkIt;
      handleNextEvent();
      handled = true;
      break;
    }
    // if we are before a link
    if (m_paintPosition <= l->textStart())
    {
      // this link is ours. (since m_links is in order)
      m_currentLink = linkIt;
      m_nextEvent = l->textStart();
      m_nextEventType = l->eventType();
      handled = true;
      break;
    }
  }
  if (not handled)
  {
    m_currentLink = m_links.end();
    m_nextEvent = m_documentSize;
    m_nextEventType = Link::STATE_PLAIN;
  }
}

int RichTextArea::lineAt(int y) const
{
  int dy = (y - m_bounds.y);
  int lineNum = 0;
  for (; dy > 0;)
  {
    LineHeightMap::const_iterator it(m_lineHeight.find(lineNum));

    if (m_lineHeight.end() != it)
    {
      dy -= it->second;
    }
    else
    {
      dy -= font().height();
    }
    lineNum++;
  }
  lineNum--;
  return lineNum;
}

int RichTextArea::pointToCharIndex(int x, int y) const
{
  int lineNum = lineAt(y);
  unsigned int currentChildIndex = 0;
  unsigned int charNumber = documentSize(lineNum, &currentChildIndex);
  const UnicodeString & line(m_document[lineNum]);
  int caretChar = -1;
  bool hasComponent(lineHasComponent(lineNum));
  for (int i = 0, size = 0; i < (int)line.length(); ++i, ++charNumber)
  {
    // check if component here too
    while (hasComponent and currentChildIndex < m_childPositions.size()
        and charNumber == m_childPositions[currentChildIndex])
    {
      Component * c(m_children[currentChildIndex]);
      ++currentChildIndex;
      // if the bounds of the component are smaller than the scrollpane, clip to the component.
      if (y >= c->y() and (y <= (c->y()+c->width())))
      {
        size += c->width();
      }
      if (size > x)
      {
        // gone past x, not here
        caretChar = -1;
        goto done;
      }
    }

    unsigned int value(line[i]);
    if (value == UTF8::MALFORMED)
      value = '?';
    Font::Glyph g;
    font().glyph(value, g);
    size += g.width;
    if (size > x)
    {
      size -= g.width;
      caretChar = i;
      break;
    }
  }
done:
  if (caretChar != -1 and lineNum > 0)
  {
    int charsToLine = documentSize(lineNum);
    caretChar += charsToLine;
  }
  return caretChar;
}

bool RichTextArea::childTouch(int x, int y)
{
  bool hit(false);
  for (std::vector<Component*>::iterator it(m_children.begin());
      it != m_children.end();
      ++it)
  {
    Component * child(*it);
    hit |= child->touch(x, y);
  }
  return hit;
}

bool RichTextArea::touch(int x, int y)
{
  if ( m_bounds.hit(x, y))
  {
    int charClicked = pointToCharIndex(x, y);
    if (charClicked == -1) {
      return childTouch(x, y);
    }

    // now see if this is in a link
    LinkList::const_iterator linkIt(m_links.begin());
    for (; linkIt != m_links.end(); ++linkIt)
    {
      Link * l(*linkIt);
      if (    ((unsigned int)charClicked) >= l->textStart()
          and ((unsigned int)charClicked) <= l->textEnd())
      {
        if (l->eventType() == Link::STATE_LINK)
        {
          l->setClicked();
        }
        if (m_linkListener)
        {
          m_linkListener->linkClicked(l);
        }
        break;
      }
    }
    return true;
  }
  return false;
}

void RichTextArea::addLinkListener(LinkListener * linkListener)
{
  m_linkListener = linkListener;
}


void RichTextArea::insertNewline()
{
  bool pnl = parseNewline();
  setParseNewline();
  RichTextArea::appendText(string2unicode("\n"));
  setParseNewline(pnl);
}

bool RichTextArea::centred() const
{
  return m_centred;
}

void RichTextArea::setCentred(bool centre)
{
  m_centred = centre;
}

bool RichTextArea::outlined() const
{
  return m_outlined;
}

void RichTextArea::setOutlined(bool outline)
{
  m_outlined = outline;
}

