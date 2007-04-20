#include "HtmlInputElement.h"

const UnicodeString * HtmlInputElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "type")
  {
    return &m_type;
  }
  if (name == "value")
  {
    return &m_value;
  }
  if (name == "name")
  {
    return &m_name;
  }
  if (name == "size")
  {
    return &m_size;
  }
  return 0;

}
