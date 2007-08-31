#include "Canvas.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "TextField.h"
#include "TextListener.h"
#include "Palette.h"

const static nds::Color EDGE(20,20,20);
const static nds::Color SHADOW(28,28,28);
// UnicodeString s = L"\u1234";

TextField::TextField(const UnicodeString & text) :
  TextContainer(text)
{ 
}

void TextField::paint(const nds::Rectangle & clip)
{
  TextContainer::paint(clip);

  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.top(), m_bounds.w, EDGE);
  nds::Canvas::instance().verticalLine(m_bounds.left(), m_bounds.top(), m_bounds.h, EDGE);
  nds::Canvas::instance().horizontalLine(m_bounds.x, m_bounds.bottom()-1, m_bounds.w, SHADOW);
  nds::Canvas::instance().verticalLine(m_bounds.right(), m_bounds.top(), m_bounds.h, SHADOW);
}

bool TextField::touch(int x, int y)
{
  if (m_bounds.hit(x, y))
  {
    if (listener())
    {
      listener()->editText(this);
    }
    return true;
  }
  return false;
}

