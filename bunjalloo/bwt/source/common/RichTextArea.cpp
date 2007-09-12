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
#include "Link.h"
#include "LinkListener.h"
#include "Palette.h"
#include "RichTextArea.h"
#include "UTF8.h"
#include "Rectangle.h"
#include "Canvas.h"
#include "WidgetColors.h"
using namespace std;
using namespace nds;
const static unsigned char NEWLINE('\n');

RichTextArea::RichTextArea(Font * font) :
  TextArea(font),
  m_documentSize(0),
  m_state(STATE_PLAIN),
  m_linkListener(0)
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
  m_state = STATE_PLAIN;
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

/*
void RichTextArea::appendText_copyPaste(const UnicodeString & unicodeString)
{
  // append text, adding in new lines as needed to wrap.
  int currPosition = 0;
  // find the next space character
  UnicodeString::const_iterator it(unicodeString.begin());
  for (; it != unicodeString.end();)
  {
    const UnicodeString word(nextWord(unicodeString, currPosition));
    int size = textSize(word);

    // if the word ends with a new line, then increment the height.
    // otherwise, if we go off the end of the line, increment the height.
    printf("Append word %d, m_appendPosition %d, size %d\n", word.size(), m_appendPosition, size);
    if ((m_appendPosition + size) > width())
    {
      // trim spaces from the end of the line
      // this word overflows the line - make a new line to hold the text.
      m_document.push_back(UnicodeString());
      // m_document += NEWLINE;
      m_appendPosition = 0;
      m_preferredHeight += font().height();
      printf("Word overshot\n");
    }
    if (m_preferredWidth < 0)
      m_preferredWidth = 0;
    m_preferredWidth += size;
    currentLine().append(word);
    m_appendPosition += size;
    advanceWord(unicodeString, word.length(), currPosition, it);

    // if the word ended in a NEWLINE, then go onto the next line.
    if (parseNewline() and word[word.length()-1] == NEWLINE)
    {
      m_appendPosition = 0;
      m_document.push_back(UnicodeString());
      m_preferredHeight += font().height();
    }
  }
  //m_preferredHeight = m_document.size() * font().height();
}
*/

void RichTextArea::appendText(const UnicodeString & unicodeString)
{
  if (m_document.size() > 1 and isEmpty(unicodeString) and isEmpty(currentLine()) and isEmpty(m_document[m_document.size()-2]))
  {
    // avoid duplicate empty lines
    return;
  }
  // I'm going to have to copy+paste TextArea::appendText here
  // Then check that there isn't a "Link" <change that name> within the text
  // If there is, then shuffle the text about to take it into account 
  // (increment m_appendPosition, if it goes off the right of bounds then start a new line, 
  // if a line contains a component, then remember the tallest one and move the next line a suitable amount)
  TextArea::appendText(unicodeString);
  // appendText_copyPaste(unicodeString);
  m_documentSize = documentSize();
}

void RichTextArea::add(Component * child)
{
  Component::add(child);
  // ulp - need to do what ScrollPane does.
  // also, when someone calls setLocation on this, need to shuffle children up that amount
  // - i.e. keep track of child component positions too
  // x position should be m_appendPosition + gap
  // y position should be m_document.size() * font height 
  // - no! that^ plus any oversize lines caused by other components.. oh fudge.. i'd have to keep track of
  // line-height on a per line bases
  
  Rectangle bounds(child->preferredSize());
  child->setSize(bounds.w, bounds.h);
  if (m_bounds.w <= bounds.w)
    child->setSize(m_bounds.w, bounds.h);
  else
    child->setSize(bounds.w, bounds.h);


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
  // no! doesn't take previous into account
  // int y = font().height()* (m_document.size()-1);
  int y = 0;
  for (unsigned int i = 0; i < m_document.size()-1; ++i)
  {
    if (lineHasComponent(i))
      y += m_lineHeight[i];
    else
      y += font().height();
  }
  printf("Add Child at %d %d (%d, line %d)\n", x, y, m_documentSize, m_document.size());
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

void RichTextArea::setSize(unsigned int w, unsigned int h)
{
  printf("Set size %d %d\n", w, h);
  Component::setSize(w, h);
  // re-layout children.. uf, nightmare.
}

void RichTextArea::addLink(const std::string & href)
{
  Link * link = new Link(href);
  link->setTextStart(m_documentSize);
  m_links.push_back(link);
  m_state = STATE_LINK;
  // printf("m_links.size() %d\n", m_links.size());
}

void RichTextArea::endLink()
{
  /* Fixes crash bug issue 20... but isn't a good solution */
  if (m_state == STATE_LINK)
  {
    m_state = STATE_PLAIN;
    // printf("m_links.size() %d\n", m_links.size());
    Link & link(*m_links.back());
    link.setTextEnd(m_documentSize);
    if (link.textEnd() == link.textStart())
    {
      printf("pop link endLink\n");
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

void RichTextArea::setColor(unsigned short color)
{
  if (m_state == STATE_LINK)
  {
    // adjust the last link to be this color
    // e.g. an image that is a link. => green text [like this] but also a link.
    // i.e. may have some text and an image inside a link.
    // how can I do this? it appears to be IMPOSSIBLE. Or at least require a million flags and states.
    // when coupled with bold, etc, it is going to piss me right off.
    // some link <bold, but still a link> [an image.png] end link
    // The worst of all this, is that the state is already stored correctly in the document tree, 
    // but I can't use that here because it deals in rows newlines and spaces.
    string href = m_links.back()->href();
    endLink();
    addLink(href);
    m_links.back()->setColor(color);
  }
  else
  {
    Link * link = new Link(color);
    link->setTextStart(m_documentSize);
    m_links.push_back(link);
    m_state = STATE_COLOR;
  }
}

void RichTextArea::endColor()
{
  if (m_state == STATE_COLOR)
  {
    m_state = STATE_PLAIN;
    Link & link(*m_links.back());
    link.setTextEnd(m_documentSize);
    if (link.textEnd() == link.textStart())
    {
    printf("pop link endCol\n");
      m_links.pop_back();
      delete &link;
    }
  }
  else if (m_state == STATE_LINK)
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

  if (lineHasComponent(m_lineNumber))
  {
    // take care - can have text first or component first
    UnicodeString::const_iterator it(unicodeString.begin());
    static const UnicodeString delimeter(string2unicode(" \r\n	"));
    unsigned int lastPosition = unicodeString.find_last_not_of(delimeter);
    unsigned int i = 0;

    for (; it != unicodeString.end() ; ++it, ++i)
    {
      while (true)
      {
        if (m_currentChildIndex < m_childPositions.size() and m_paintPosition == m_childPositions[m_currentChildIndex])
        {
          Component * c(m_children[m_currentChildIndex]);
          //c->setLocation(m_cursorx, m_cursory);
          /*
          Rectangle thisClip(m_clip->intersect(c->bounds()));
          // if the bounds of the component are smaller than the scrollpane, clip to the component.
          if (thisClip.w != 0 and thisClip.h != 0) {
            c->paint(thisClip);
            printf("Paint Child %d at %d %d, cursor pos %d %d\n", m_currentChildIndex, c->x(), c->y(), m_cursorx, m_cursory);

            Canvas::instance().setClip(*m_clip);
          }
          */
          if (m_cursory == c->y())
          {
            m_cursorx += c->width();
          }
          printf("cursorx %d\n", m_cursorx);
          ++m_currentChildIndex;
        }
        else
        {
          break;
        }
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
    /*
    Component * currentChild(0);
    if (m_currentChildIt != m_children.end())
    {
      currentChild = *m_currentChildIt;
    }
    for (; it != unicodeString.end() ; ++it, ++i)
    {
      while (true)
      {
        if (currentChild and m_cursory == currentChild->y() and m_cursorx >= currentChild->x())
        {
          m_cursorx += currentChild->width();
          ++m_currentChildIt;
          if (m_currentChildIt == m_children.end())
            currentChild = 0;
          else
            currentChild = *m_currentChildIt;
        }
        else
        {
          break;
        }
      }
      if (m_nextEvent == m_paintPosition)
      {
        handleNextEvent();
      }
      m_paintPosition++;
      if (i > lastPosition)
        continue;
      unsigned int value(*it);
      if ( doSingleChar(value) )
      {
        break;
      }
    }
    */

  }
  else
  {
    // old code - no component on this line, so can print without fear
    UnicodeString::const_iterator it(unicodeString.begin());
    static const UnicodeString delimeter(string2unicode(" \r\n	"));
    unsigned int lastPosition = unicodeString.find_last_not_of(delimeter);
    unsigned int i = 0;
    for (; it != unicodeString.end() /*and cursorY() < m_bounds.bottom()*/; ++it, ++i)
    {
      if (m_nextEvent == m_paintPosition)
      {
        handleNextEvent();
      }
      m_paintPosition++;
      if (i > lastPosition)
        continue;
      unsigned int value(*it);
      if ( doSingleChar(value) )
      {
        break;
      }
    }
  }
}
void RichTextArea::handleNextEvent()
{
  switch (m_nextEventType)
  {
    case STATE_LINK:
      if ( (*m_currentLink)->clicked() )
      {
        setTextColor(WidgetColors::LINK_CLICKED);
      }
      else
      {
        // setTextColor(WidgetColors::LINK_REGULAR);
        setTextColor((*m_currentLink)->color());
      }
      m_nextEvent = (*m_currentLink)->textEnd();
      (*m_currentLink)->setClicked(false);
      m_nextEventType = STATE_PLAIN;
      setUnderline();
      break;
    case STATE_PLAIN:
      setTextColor(0);//Color(0,0,0));
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
        m_nextEventType = STATE_PLAIN;
      }
      break;
    case STATE_COLOR:
      setTextColor((*m_currentLink)->color());
      m_nextEvent = (*m_currentLink)->textEnd();
      (*m_currentLink)->setClicked(false);
      m_nextEventType = STATE_PLAIN;
      setUnderline(false);
      break;
  }
}

int RichTextArea::documentSize(int endLine) const
{
  int total = 0;
  if (endLine == -1)
  {
    endLine = m_document.size();
  }
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  for (int i = 0; it != m_document.end() and i != endLine; ++it, ++i)
  {
    total += it->length();
  }
  return total;
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
    m_nextEventType = STATE_PLAIN;
  }
  m_paintPosition = 0;
  m_lineNumber = 0;
  // work out what happens when we skip lines.
  int skipLines(linesToSkip());
  m_currentChildIndex = 0;
  if (skipLines > 0)
  {
    checkSkippedLines(skipLines);
    m_lineNumber = skipLines;
  }
  /*
  m_currentChildIt = m_children.begin();
  if (not m_children.empty())
  {
    // skip until m_currentChildIt points at a child that is >= skip lines * font height
    int targetLine = skipLines * font().height();
    for ( ; m_currentChildIt != m_children.end() ; ++m_currentChildIt)
    {
      // check this position
      Component * c(*m_currentChildIt);
      if (c->y() >= (m_bounds.y + targetLine) )
      {
        break;
      }
    }
  }
  */
  m_clip = &clip;

  TextArea::paint(clip);

  
  for (std::vector<Component*>::iterator it(m_children.begin());
      it != m_children.end(); 
      ++it)
  {
    Component * c(*it);
    Rectangle bounds(c->bounds());
    Rectangle thisClip(clip.intersect(bounds));
    // if the bounds of the component are smaller than the scrollpane, clip to the component.
    if (thisClip.w == 0 and thisClip.h == 0)
      continue;
    c->paint(thisClip);
    nds::Canvas::instance().setClip(clip);
  }

}

void RichTextArea::checkSkippedLines(int skipLines)
{
  // the lines are skipped - see which link applies to us, if any
  unsigned int total = 0;
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  for (int i = 0; it != m_document.end() and i != skipLines; ++it, ++i)
  {
    total += it->length();
    // skip to the next child position
    if (m_currentChildIndex < m_childPositions.size() and total > m_childPositions[m_currentChildIndex])
    {
      ++m_currentChildIndex;
    }
  }
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
    m_nextEventType = STATE_PLAIN;
  }
}

int RichTextArea::pointToCharIndex(int x, int y) const
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
  printf("lineNum = %d\n", lineNum);
  //int lineNum = (y - m_bounds.y)/font().height();
  unsigned int charNumber = 0;
  unsigned int currentChildIndex = 0;
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  for (int i = 0; it != m_document.end() and i != lineNum; ++it, ++i)
  {
    charNumber += it->size();
    if (currentChildIndex < m_childPositions.size() and charNumber > m_childPositions[currentChildIndex])
    {
      ++currentChildIndex;
    }
  }
  const UnicodeString & line(m_document[lineNum]);
  int caretChar = -1;
  int size = 0;
  for (int i = 0; i < (int)line.length(); ++i, ++charNumber)
  {

    // check if component here too
    while (true)
    {
      if (currentChildIndex < m_childPositions.size() and charNumber == m_childPositions[currentChildIndex])
      {
        Component * c(m_children[currentChildIndex]);
        //c->setLocation(m_cursorx, m_cursory);
        Rectangle thisClip(m_clip->intersect(c->bounds()));
        // if the bounds of the component are smaller than the scrollpane, clip to the component.
        if (y >= c->y() and (y <= (c->y()+c->width())))
        {
          size += c->width();
        }
        ++currentChildIndex;
        if (size > x)
        {
          // gone past x, not here
          caretChar = -1;
          goto done;
        }
      }
      else
      {
        break;
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
  printf("caretChar %d\n", caretChar);
  if (caretChar == -1)
  {
    // caretChar = line.length();
    return -1;
  }
  // now we know the line and the character in that line.
  if (lineNum > 0)
  {
    int charsToLine = documentSize(lineNum);
    caretChar += charsToLine;
  }
  printf("return caretChar %d\n", caretChar);
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
    if (charClicked == -1)
      return childTouch(x, y);
    // now see if this is in a link
    LinkList::const_iterator linkIt(m_links.begin());
    for (; linkIt != m_links.end(); ++linkIt)
    {
      Link * l(*linkIt);
      if (l->eventType() == Link::STATE_LINK
          and ((unsigned int)charClicked) >= l->textStart()
          and ((unsigned int)charClicked) <= l->textEnd())
      {
        l->setClicked();
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
