#include "HtmlImageElement.h"

const UnicodeString * HtmlImageElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }

  if (name == "src")
  {
    return &m_src;
  }
  if (name == "alt")
  {
    return &m_alt;
  }
  if (name == "name")
  {
    return &m_name;
  }
  return 0;

}

void HtmlImageElement::copyAttributes(HtmlElement * copyTo) const
{
  HtmlElement::copyAttributes(copyTo);
  ((HtmlImageElement*)copyTo)->m_src = m_src;
  ((HtmlImageElement*)copyTo)->m_alt = m_alt;
  ((HtmlImageElement*)copyTo)->m_name = m_name;
}

HtmlElement * HtmlImageElement::clone() const
{
  HtmlElement * theClone(new HtmlImageElement(m_tagName));
  copyAttributes(theClone);
  return theClone;
}

void HtmlImageElement::setAttribute(const std::string & name, const UnicodeString & value)
{
  HtmlElement::setAttribute(name, value);
  if (name == "alt")
  {
    m_hasAltText = true;
  }
}

bool HtmlImageElement::hasAltText() const
{
  return m_hasAltText;
}
