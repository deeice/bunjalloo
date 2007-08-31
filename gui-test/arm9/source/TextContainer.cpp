#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextContainer.h"

static const int BORDER_WIDTH(5);
static const int BORDER_HEIGHT(5);

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
    //w -= BORDER_WIDTH;
  }
  else
  {
  }
  textArea()->setSize(w, h);
  m_preferredHeight = textArea()->preferredSize().h+BORDER_HEIGHT;
  layout();
}

void TextContainer::layout()
{
  if (textArea()->width() == 0)
  {
    textArea()->setSize(textArea()->textSize(m_text), textArea()->font().height());
  }
  UnicodeString appendText;
  appendText.clear();
  UnicodeString::const_iterator it(m_text.begin());
  int size(0);
  for (; it != m_text.end() and size < textArea()->width(); ++it)
  {
    Font::Glyph g;
    textArea()->font().glyph(*it, g);
    if ((size + g.width) < textArea()->width()) {
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
