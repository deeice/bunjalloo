/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "HtmlTextAreaElement.h"
#include "Visitor.h"
IMPLEMENT_ACCEPT(HtmlTextAreaElement);

static const int MINIMUM_ROWS(4);
static const int MINIMUM_COLS(10);
const UnicodeString * HtmlTextAreaElement::attributePtr(const std::string & name) const
{
  const UnicodeString * p = HtmlElement::attributePtr(name);
  if (p) {
    return p;
  }
  if (name == "name")
  {
    return &m_name;
  }
  return 0;
}

void HtmlTextAreaElement::setAttribute(const std::string & name,
    const UnicodeString & value)
{
  HtmlElement::setAttribute(name, value);
  if (name == "rows")
  {
    m_rows = strtol( unicode2string(value).c_str(), 0, 10);
  }
  if (name == "cols")
  {
    m_cols = strtol( unicode2string(value).c_str(), 0, 10);
  }
}
int HtmlTextAreaElement::rows() const
{
  if (m_rows < MINIMUM_ROWS)
  {
    return MINIMUM_ROWS;
  }
  return m_rows;
}

int HtmlTextAreaElement::cols() const
{
  if (m_cols < MINIMUM_COLS)
  {
    return MINIMUM_COLS;
  }
  return m_cols;
}
