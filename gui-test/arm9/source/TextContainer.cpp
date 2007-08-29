#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextContainer.h"

static const int BORDER_WIDTH(5);
static const int BORDER_HEIGHT(5);

TextContainer::TextContainer(const UnicodeString & text) :
  Component(),
  m_textArea(TextAreaFactory::create())
{
  m_textArea->setSize(m_textArea->textSize(text), m_textArea->font().height());
  nds::Rectangle textSize = m_textArea->preferredSize();
  m_preferredWidth = textSize.w+BORDER_WIDTH;
  m_preferredHeight = textSize.h+BORDER_HEIGHT;
  setText(text);
}


TextContainer::TextContainer()
{}

TextContainer::~TextContainer()
{
  delete m_textArea;
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
    m_preferredWidth = w;
    w -= BORDER_WIDTH;
  }
  else
  {
  }
  m_textArea->setSize(w, h);
  m_preferredHeight = m_textArea->preferredSize().h+BORDER_HEIGHT;
  layout();
}

void TextContainer::layout()
{
  UnicodeString appendText;
  appendText.clear();
  UnicodeString::const_iterator it(m_text.begin());
  int size(0);
  for (; it != m_text.end() and size < m_textArea->width(); ++it)
  {
    Font::Glyph g;
    m_textArea->font().glyph(*it, g);
    if ((size + g.width) < m_textArea->width()) {
      size += g.width;
      appendText += *it;
    } else {
      break;
    }
  }
  m_textArea->clearText();
  m_textArea->appendText(appendText);
  if (size < m_bounds.w)
  {
    m_bounds.w = size + BORDER_WIDTH;
  }
}

void TextContainer::paint(const nds::Rectangle & clip)
{
  m_textArea->setLocation(m_bounds.x+BORDER_WIDTH/2, m_bounds.y+BORDER_HEIGHT/2);
  m_textArea->paint(clip);
}

void TextContainer::setBackgroundColor(unsigned short color)
{
  m_textArea->setBackgroundColor(color);
}

const UnicodeString & TextContainer::text() const
{
  return m_text;
}
