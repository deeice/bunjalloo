#include "Canvas.h"
#include "Palette.h"
#include "PasswordField.h"
#include "Rectangle.h"
#include "TextArea.h"
#include "HtmlElement.h"

using nds::Canvas;
using nds::Color;
using std::string;

PasswordField::PasswordField(HtmlElement * element, const TextArea * textArea)
  : InputText(element, textArea)
{ }

void PasswordField::draw(TextArea * textArea)
{
  FormControl::draw(textArea);
  UnicodeString text;
  UnicodeString value(m_element->attribute("value"));
  UnicodeString::const_iterator it(value.begin());
  int size(0);
  Font::Glyph g;
  textArea->font().glyph('*', g);
  for (; it != value.end() and size < m_size->w; ++it)
  {
    if (size + g.width < m_size->w) {
      size += g.width;
      text += *it;
    } else {
      break;
    }
  }
  UnicodeString pass(text.length(), '*');
  textArea->printu(pass);
}
