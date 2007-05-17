#include "Canvas.h"
#include "Palette.h"
#include "Button.h"
#include "Rectangle.h"
#include "TextArea.h"
#include "HtmlElement.h"

using nds::Canvas;
using nds::Color;

Button::Button(HtmlElement * element, const TextArea * textArea)
  : FormControl(element)
{
  int size = textArea->textSize(m_element->attribute("value"));
  if ( size > m_size->w)
  {
    if (size > MAX_SIZE)
      size = MAX_SIZE;
    m_size->w = size;
  }
}

void Button::draw(TextArea * textArea)
{
  FormControl::draw(textArea);
  Canvas::instance().fillRectangle(m_size->x+1, m_size->y+1, m_size->w-1, m_size->h-1, Color(26,26,30));
  // print the button
  textArea->setBackgroundColor(Color(26,26,30));
  textArea->printu(m_element->attribute("value"));
  textArea->setBackgroundColor(Color(31,31,31));
}

