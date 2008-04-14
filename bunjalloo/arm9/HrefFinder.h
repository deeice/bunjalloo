/*
  Copyright (C) 2008 Richard Quirk

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
#ifndef HrefFinder_h_seen
#define HrefFinder_h_seen
#include "DefaultVisitor.h"
#include "UnicodeString.h"

class HrefFinder: public DefaultVisitor
{
  public:
    HrefFinder(const std::string & href);

    inline bool found() const
    {
      return m_element != 0;
    }

    bool visit(HtmlAnchorElement & element);

    inline HtmlElement * element() const
    {
      return m_element;
    }

  private:
    HtmlElement * m_element;
    UnicodeString m_href;
};
#endif
