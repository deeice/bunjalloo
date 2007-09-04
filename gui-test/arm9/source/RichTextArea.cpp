#include "RichTextArea.h"
#include "Link.h"
#include "Palette.h"
#include "UTF8.h"
#include "WidgetColors.h"

RichTextArea::RichTextArea(Font * font) :
  TextArea(font),
  m_currentPosition(0),
  m_state(STATE_TEXT)
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

void RichTextArea::appendText(const UnicodeString & unicodeString)
{
  m_currentPosition += unicodeString.length();
  TextArea::appendText(unicodeString);
  m_documentSize = documentSize();
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
  m_state = STATE_TEXT;
  Link & link(*m_links.back());
  link.setTextEnd(m_currentPosition);
}

void RichTextArea::printu(const UnicodeString & unicodeString)
{
  UnicodeString::const_iterator it(unicodeString.begin());
  for (; it != unicodeString.end() and cursorY() < m_bounds.bottom(); ++it)
  {
    if (m_nextEvent == m_paintPosition)
    {
      handleNextEvent();
    }
    m_paintPosition++;
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
        setTextColor(WidgetColors::LINK_REGULAR);
      }
      m_nextEvent = (*m_currentLink)->textEnd();
      (*m_currentLink)->setClicked(false);
      m_nextEventType = STATE_TEXT;
      setUnderline();
      break;
    case STATE_TEXT:
      setTextColor(0);//Color(0,0,0));
      setUnderline(false);
      if (m_currentLink != m_links.end())
        ++m_currentLink;
      if (m_currentLink != m_links.end())
      {
        m_nextEvent = (*m_currentLink)->textStart();
        m_nextEventType = STATE_LINK;
      } else {
        m_nextEvent = m_documentSize;
        m_nextEventType = STATE_TEXT;
      }
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
  // see where the first link is
  m_nextEvent = 0;
  if (not m_links.empty())
  {
    m_currentLink = m_links.begin();
    m_nextEvent = (*m_currentLink)->textStart();
    m_nextEventType = STATE_LINK;
  }
  else
  {
    m_currentLink = m_links.end();
    m_nextEvent = m_documentSize;
    m_nextEventType = STATE_TEXT;
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
      m_nextEventType = STATE_LINK;
      m_currentLink = linkIt;
      handleNextEvent();
      handled = true;
      break;
    }
    // if we pass a link:
    if (m_paintPosition >= l->textEnd())
    {
      // the next link is ours.
      m_currentLink = linkIt;
      m_nextEventType = STATE_TEXT;
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
      m_nextEventType = STATE_LINK;
      handled = true;
      break;
    }
  }
  if (not handled)
  {
    m_currentLink = m_links.end();
    m_nextEvent = m_documentSize;
    m_nextEventType = STATE_TEXT;
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
      if (charClicked >= l->textStart() and charClicked <= l->textEnd())
      {
        l->setClicked();
        break;
      }
    }
    return true;
  }
  return false;
}
