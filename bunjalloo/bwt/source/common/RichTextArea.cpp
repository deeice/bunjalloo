#include "Link.h"
#include "LinkListener.h"
#include "Palette.h"
#include "RichTextArea.h"
#include "UTF8.h"
#include "WidgetColors.h"
using namespace std;

RichTextArea::RichTextArea(Font * font) :
  TextArea(font),
  m_currentPosition(0),
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
  m_currentPosition = m_documentSize;
}

void RichTextArea::addLink(const std::string & href)
{
  Link * link = new Link(href);
  link->setTextStart(m_currentPosition);
  m_links.push_back(link);
  m_state = STATE_LINK;
}

void RichTextArea::endLink()
{
  m_state = STATE_PLAIN;
  Link & link(*m_links.back());
  link.setTextEnd(m_currentPosition);
  if (link.textEnd() == link.textStart())
  {
    m_links.pop_back();
    delete &link;
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
    link->setTextStart(m_currentPosition);
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
    link.setTextEnd(m_currentPosition);
    if (link.textEnd() == link.textStart())
    {
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

void RichTextArea::printu(const UnicodeString & unicodeString)
{
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

  // work out what happens when we skip lines.
  int skipLines(linesToSkip());
  if (skipLines > 0)
  {
    checkSkippedLines(skipLines);
  }

  TextArea::paint(clip);

}

void RichTextArea::checkSkippedLines(int skipLines)
{
  // the lines are skipped - see which link applies to us, if any
  int total = 0;
  std::vector<UnicodeString>::const_iterator it(m_document.begin());
  for (int i = 0; it != m_document.end() and i != skipLines; ++it, ++i)
  {
    total += it->length();
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
  int lineNum = (y - m_bounds.y)/font().height();
  const UnicodeString & line(m_document[lineNum]);
  int caretChar = -1;
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
      caretChar = i;
      break;
    }
  }
  if (caretChar == -1)
  {
    caretChar = line.length();
  }
  // now we know the line and the character in that line.
  if (lineNum > 0)
  {
    int charsToLine = documentSize(lineNum);
    caretChar += charsToLine;
  }
  return caretChar;
}

bool RichTextArea::touch(int x, int y)
{
  if ( m_bounds.hit(x, y))
  {
    unsigned int charClicked = pointToCharIndex(x, y);
    // now see if this is in a link
    LinkList::const_iterator linkIt(m_links.begin());
    for (; linkIt != m_links.end(); ++linkIt)
    {
      Link * l(*linkIt);
      if (l->eventType() == Link::STATE_LINK and charClicked >= l->textStart() and charClicked <= l->textEnd())
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
