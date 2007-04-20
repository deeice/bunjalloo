#include "HtmlAnchorElement.h"

const UnicodeString * HtmlAnchorElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "href")
  {
    return &m_href;
  }
  return 0;

}

void HtmlAnchorElement::copyAttributes(HtmlElement * copyTo) const
{
  HtmlElement::copyAttributes(copyTo);
  ((HtmlAnchorElement*)copyTo)->m_href = m_href;
}

HtmlElement * HtmlAnchorElement::clone() const
{
  HtmlElement * theClone(new HtmlAnchorElement(m_tagName));
  copyAttributes(theClone);
  return theClone;
}
