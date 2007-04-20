#include "HtmlMetaElement.h"

HtmlMetaElement::HtmlMetaElement(const std::string & tagName) 
: HtmlElement(tagName)
{}

const UnicodeString * HtmlMetaElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "http-equiv")
  {
    return &m_httpEquiv;
  }
  if (name == "content")
  {
    return &m_content;
  }
  return 0;

}

void HtmlMetaElement::copyAttributes(HtmlElement * copyTo) const
{
  HtmlElement::copyAttributes(copyTo);
  ((HtmlMetaElement*)copyTo)->m_httpEquiv = m_httpEquiv;
  ((HtmlMetaElement*)copyTo)->m_content = m_content;
}

HtmlElement * HtmlMetaElement::clone() const
{
  HtmlElement * theClone(new HtmlMetaElement(m_tagName));
  copyAttributes(theClone);
  return theClone;
}

