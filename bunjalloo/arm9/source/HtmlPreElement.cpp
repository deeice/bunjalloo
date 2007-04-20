#include "HtmlPreElement.h"


void HtmlPreElement::appendText(unsigned int value)
{
  if (m_children.size())
  {
    if (m_children.back()->isa("#TEXT"))
    {
      m_children.back()->text() += value;
      return;
    }
  }
  HtmlElement* textNode = new HtmlElement("#TEXT");
  textNode->text() = value;
  append(textNode);
}
