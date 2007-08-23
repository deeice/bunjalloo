/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
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
