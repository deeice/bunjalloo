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
#include "Cookie.h"
#include "CookieJar.h"
#include "ParameterSet.h"
#include "URI.h"

using namespace std;

CookieJar::CookieJar()
{
}

inline void deleteCookie(Cookie * c)
{
  delete c;
}

CookieJar::~CookieJar()
{
  for_each(m_cookies.begin(), m_cookies.end(), deleteCookie);
  m_cookies.clear();
}


bool CookieJar::hasCookieForDomain(const URI & uri, const string & name) const
{
  vector<Cookie *>::const_iterator it(m_cookies.begin());
  vector<Cookie *>::const_iterator end(m_cookies.end());
  for (; it != end; ++it)
  {
    Cookie * c(*it);
    if (c->matchesDomain(uri.server()) and c->name() == name)
    {
      return true;
    }
  }
  return false;
}

void CookieJar::addCookieHeader(const URI & uri, const std::string & request)
{
  int port = uri.port();
  string domain = uri.server();
  bool domainInitialDot = false; // FIXME! what does it mean??
  string path = uri.fileName();

  if (not acceptCookies(domain))
  {
    // this server is blocked.
    return;
  }

  ParameterSet paramSet(request);
  const KeyValueMap & keyValueMap(paramSet.keyValueMap());
  if ( paramSet.hasParameter("path") )
  {
    paramSet.parameter("path", path);
  }
  
  for (KeyValueMap::const_iterator it(keyValueMap.begin()); it != keyValueMap.end(); ++it)
  {
    string name = it->first;
    if (name == "path")
    {
      continue;
    }
    if (hasCookieForDomain(uri, name))
    {
      continue;
    }

    string value = it->second;
    Cookie * cookie = new Cookie(name, value, port, domain, domainInitialDot, path);
    // printf("set cookie for %s: %s = %s\n", domain.c_str(), name.c_str(), value.c_str());
    m_cookies.push_back(cookie);
  }
}

void CookieJar::clear(const std::string & domain,
    const std::string & path,
    const std::string & name)
{
}

void CookieJar::cookiesForRequest(const URI & request,
    std::string & headers) const
{
  string domain(request.server());
  // find a cookie in the jar that corresponds to the requested domain...
  string tmp("Cookie: ");
  bool needSep(false);
  const static string sep(";");
  std::vector<Cookie *>::const_iterator it(m_cookies.begin());
  for (; it != m_cookies.end(); ++it)
  {
    Cookie *c(*it);
    if (c->matchesDomain(domain)) {
      if (needSep)
      {
        tmp += sep;
      }
      string value(c->value());
      bool needQuotes = value.find_first_of(" ")!=string::npos;
      tmp += c->name();
      tmp += "=";
      if (needQuotes) {
        tmp += "\"";
      }
      tmp += c->value();
      if (needQuotes) {
        tmp += "\"";
      }
      needSep = true;
    }
  }
  if (needSep) {
    tmp += "\r\n";
    headers = tmp;
  }
}

bool CookieJar::acceptCookies(const std::string & domain) const
{
  AcceptedDomainMap::const_iterator it(m_acceptedDomains.find(domain));
  return (it != m_acceptedDomains.end());
}

void CookieJar::setAcceptCookies(const std::string & domain, bool accept)
{
  if (not accept and acceptCookies(domain))
  {
    // no longer accept them
    m_acceptedDomains.erase(domain);
  }

  if (accept) 
  {
    m_acceptedDomains[domain] = accept;
  }
}
