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
#ifndef HtmlTextAreaElement_h_seen
#define HtmlTextAreaElement_h_seen

#include "HtmlElement.h"
class HtmlTextAreaElement:public HtmlElement
{
  public:
    HtmlTextAreaElement(const std::string & tagName) 
      : HtmlElement(tagName), m_rows(0), m_cols(0)
    {}

    virtual void setAttribute(const std::string & name, 
        const UnicodeString & value);
    int rows() const;
    int cols() const;

  protected:
    int m_rows;
    int m_cols;
};
#endif
