#include "HtmlOptionElement.h"

const UnicodeString * HtmlOptionElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "value")
  {
    return &m_value;
  }
  if (name == "label")
  {
    return &m_label;
  }
  return 0;
}

void HtmlOptionElement::setAttribute(const std::string & name, const UnicodeString & value)
{
  if (name == "selected")
  {
    m_selected = true;
  }
  HtmlElement::setAttribute(name, value);
}

bool HtmlOptionElement::selected() const
{
  return m_selected;
}
