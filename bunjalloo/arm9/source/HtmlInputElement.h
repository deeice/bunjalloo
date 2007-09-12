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
#ifndef HtmlInputElement_h_seen
#define HtmlInputElement_h_seen
#include "HtmlElement.h"

class HtmlInputElement:public HtmlElement
{
  public:
    HtmlInputElement(const std::string & tagName) 
      : HtmlElement(tagName), m_checked(false), m_hasAltText(false)
    {}

    enum InputType
    {
      SUBMIT,
      IMAGE,
      TEXT,
      PASSWORD,
      RADIO,
      CHECKBOX,
      HIDDEN
    };
    InputType inputType() const;

    virtual void setAttribute(const std::string & name, 
        const UnicodeString & value);
    bool checked() const;
    void setChecked(bool checked);
    bool hasAltText() const;

  protected:
    UnicodeString m_type;
    UnicodeString m_value;
    UnicodeString m_name;
    UnicodeString m_alt;
    UnicodeString m_src;
    UnicodeString m_size;
    bool m_checked;
    bool m_hasAltText;
    const UnicodeString * attributePtr(const std::string & name) const;
};
#endif
