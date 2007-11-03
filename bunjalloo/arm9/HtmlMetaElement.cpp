/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

