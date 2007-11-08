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
#ifndef Cookie_h_seen
#define Cookie_h_seen

#include <string>
#include "CookieJar.h"

class Cookie
{

  public:
    Cookie()
    {}

    Cookie(const std::string & name,
           const std::string & value,
           int                 port,
           const std::string & domain,
           const std::string & path,
           bool secure
        )
      : m_name(name),
        m_value(value),
        m_port(port),
        m_domain(domain),
        m_path(path),
        m_secure(secure)
    {}

    bool matchesDomain(const std::string & domain) const
    {
      return (domain == m_domain or CookieJar::topLevel(domain) == m_domain);
    }

    const std::string & name() const
    {
      return m_name;
    }

    const std::string & value() const
    {
      return m_value;
    }

    bool secure() const
    {
      return m_secure;
    }

    const std::string & path() const
    {
      return m_path;
    }

  private:
    std::string m_name;
    std::string m_value;
    int         m_port;
    std::string m_domain;
    std::string m_path;
    bool m_secure;

};
#endif
