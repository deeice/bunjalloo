#include "HtmlFormElement.h"

const UnicodeString * HtmlFormElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "action")
  {
    return &m_action;
  }
  if (name == "method")
  {
    return &m_method;
  }
  return 0;

}
