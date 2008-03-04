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
#ifndef Cookie_h_seen
#define Cookie_h_seen

#include <string>
#include "CookieJar.h"

/** Simple implementation of cookies. */
class Cookie
{

  public:
    /** Create an empty Cookie.*/
    Cookie()
    {}

    /** Create a Cookie from parameters.
     * @param name the name of the parameter.
     * @param value the value to give it.
     * @param port the server port that set the cookie.
     * @param domain the domain name
     * @param path the path that the cookie applies to.
     * @param secure true if this cookie only applies to secure connections.
     */
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
    {
      if (m_domain[0] == '.')
      {
        m_domain = CookieJar::topLevel(m_domain);
      }
    }

    /** Check if a domain name matches this Cookie's domain field.
     * @param domain the domain name to check.
     * @return true if the domain name given matches for this cookie.
     */
    bool matchesDomain(const std::string & domain) const
    {
      return (domain == m_domain or CookieJar::topLevel(domain) == m_domain);
    }

    /** Get the name of the Cookie.
     * @return The name field.
     */
    const std::string & name() const
    {
      return m_name;
    }

    /** Get the value of the Cookie.
     * @return the value.
     */
    const std::string & value() const
    {
      return m_value;
    }

    /** Set the Cookie's value.
     * @param value the value string.
     */
    void setValue(const std::string & value)
    {
      m_value = value;
    }

    /** Get the secure status.
     * @return true if this Cookie only applies to secure connections.
     */
    bool secure() const
    {
      return m_secure;
    }

    /** Get the path that this Cookie uses.
     * @return the path field.
     */
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
