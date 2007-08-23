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
#ifndef HtmlOptionElement_h_seen
#define HtmlOptionElement_h_seen
#include "HtmlElement.h"

class HtmlOptionElement : public HtmlElement
{
  public:
    HtmlOptionElement(const std::string & tagName) 
      : HtmlElement(tagName), m_selected(false) { }

    virtual void setAttribute(const std::string & name, const UnicodeString & value);

    bool selected() const;
  private:
    bool m_selected;
    UnicodeString m_value;
    UnicodeString m_label;

    const UnicodeString * attributePtr(const std::string & name) const;
};
#endif
