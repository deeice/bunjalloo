#include "Canvas.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextField.h"
#include "Palette.h"

static const int BORDER_WIDTH(5);
static const int BORDER_HEIGHT(5);
const static nds::Color EDGE(20,20,20);
const static nds::Color SHADOW(28,28,28);

TextField::TextField(const UnicodeString & text) :
  Component(),
  m_textArea(TextAreaFactory::create())
{
  m_textArea->setSize(m_textArea->textSize(text), m_textArea->font().height());
  nds::Rectangle textSize = m_textArea->preferredSize();
  m_preferredWidth = textSize.w+BORDER_WIDTH;
  m_preferredHeight = m_textArea->preferredSize().h+BORDER_HEIGHT;
  setText(text);
  layout();
}

TextField::~TextField()
{
  delete m_textArea;
}

void TextField::setSize(unsigned int w, unsigned int h)
{
  Component::setSize(w, h);
  m_preferredWidth = w;

  if ((int)w < (m_preferredWidth+BORDER_WIDTH))
  {
    w -= BORDER_WIDTH;
  }
  m_textArea->setSize(w, h);
  m_preferredHeight = m_textArea->preferredSize().h+BORDER_HEIGHT;
  layout();
}

void TextField::setText(const UnicodeString & text)
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

void TextField::layout() 
{
  UnicodeString appendText;
  appendText.clear();
  UnicodeString::const_iterator it(m_text.begin());
  int size(0);
  for (; it != m_text.end() and size < m_textArea->width(); ++it)
  {
    Font::Glyph g;
    m_textArea->font().glyph(*it, g);
    if ((size + g.width) < m_bounds.w) {
      size += g.width;
      appendText += *it;
    } else {
      break;
    }
  }
  m_textArea->clearText();
  m_textArea->appendText(appendText);
}

void TextField::paint(const nds::Rectangle & clip)
{
  m_textArea->setLocation(m_bounds.x+BORDER_WIDTH, m_bounds.y+BORDER_HEIGHT/2);
  m_textArea->paint(clip);
  //nds::Canvas::instance().drawRectangle(m_bounds.x, m_bounds.y, m_bounds.w-1, m_bounds.h-1, EDGE);
  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w, EDGE);
  nds::Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h, EDGE);
  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w, SHADOW);
  nds::Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h, SHADOW);
  // Draw a bit of shadow?
  //nds::Canvas::instance().fillRectangle(m_bounds.x+3, m_bounds.y+1, m_bounds.w-4, 2, SHADOW);
  //nds::Canvas::instance().fillRectangle(m_bounds.x+1, m_bounds.y+1, 2, m_bounds.h-2, SHADOW);
}

bool TextField::touch(int x, int y)
{
  if (m_bounds.hit(x, y))
  {
    setText(string2unicode("I've been hit!"));
    return true;
  }
  return false;
}

