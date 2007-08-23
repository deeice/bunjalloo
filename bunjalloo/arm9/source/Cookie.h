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
#ifndef Cookie_h_seen
#define Cookie_h_seen

#include <string>

class Cookie
{

  public:
    Cookie()
    {}

    Cookie(const std::string & name,
           const std::string & value,
           int                 port,
           const std::string & domain,
           bool                domainInitialDot,
           const std::string & path
        )
      : m_name(name),
        m_value(value),
        m_port(port),
        m_domain(domain),
        m_domainInitialDot(domainInitialDot),
        m_path(path)
    {}

    bool matchesDomain(const std::string & domain) const
    {
      return domain == m_domain;
    }

    const std::string & name() const
    {
      return m_name;
    }

    const std::string & value() const
    {
      return m_value;
    }

  private:
    std::string m_name;
    std::string m_value;
    int         m_port;
    std::string m_domain;
    bool        m_domainInitialDot;
    std::string m_path;

};
#endif
