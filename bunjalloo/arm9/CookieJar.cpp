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
#include "File.h"
#include "ParameterSet.h"
#include "URI.h"

using namespace std;
static const char * PATH_STR("path");
static const char * DOMAIN_STR("domain");
static const char * SECURE_STR("secure");
static const char * EXPIRES_STR("expires");

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


Cookie * CookieJar::hasCookieForDomain(const URI & uri, const std::string & name) const
{
  vector<Cookie *>::const_iterator it(m_cookies.begin());
  vector<Cookie *>::const_iterator end(m_cookies.end());
  for (; it != end; ++it)
  {
    Cookie * c(*it);
    if (c->matchesDomain(uri.server()) and c->name() == name)
    {
      return c;
    }
  }
  return 0;
}

void CookieJar::addCookieHeader(const URI & uri, const std::string & request)
{
  // printf("addCookieHeader: %s \nFrom: %s\n",request.c_str(), uri.asString().c_str());
  int port = uri.port();
  string domain = uri.server();
  string top(topLevel(domain));
  string path = uri.fileName();
  bool secure(false);

  if (not acceptCookies(domain))
  {
    // this server is blocked.
    return;
  }
  string lowCaseRequest(request);
  transform(lowCaseRequest.begin(), lowCaseRequest.end(), lowCaseRequest.begin(), ::tolower);

  ParameterSet paramSet(request);
  ParameterSet lowCaseParamSet(lowCaseRequest);

  if ( lowCaseParamSet.hasParameter(PATH_STR) )
  {
    lowCaseParamSet.parameter(PATH_STR, path);
  }
  else
  {
    // sort out basename
    if (path[path.length()-1] != '/')
    {
      path = nds::File::dirname(path.c_str());
    }
  }
  if ( lowCaseParamSet.hasParameter(DOMAIN_STR) )
  {
    lowCaseParamSet.parameter(DOMAIN_STR, domain);
  }
  if ( lowCaseParamSet.hasParameter(SECURE_STR) )
  {
    secure = true;
  }

  const KeyValueMap & keyValueMap(paramSet.keyValueMap());
  for (KeyValueMap::const_iterator it(keyValueMap.begin()); it != keyValueMap.end(); ++it)
  {
    string name = it->first;
    string lowCaseName(name);
    transform(lowCaseName.begin(), lowCaseName.end(), lowCaseName.begin(), ::tolower);
    if (lowCaseName == PATH_STR or lowCaseName == SECURE_STR
        or lowCaseName == EXPIRES_STR or lowCaseName == DOMAIN_STR)
    {
      continue;
    }
    Cookie * existingCookie = hasCookieForDomain(uri, name);
    if (existingCookie != 0)
    {
      // replace with new value
      existingCookie->setValue(it->second);
    }
    else
    {
      string value = it->second;
      Cookie * cookie = new Cookie(name, value, port, domain, path, secure);
      // printf("set cookie for %s: %s = %s\n", domain.c_str(), name.c_str(), value.c_str());
      m_cookies.push_back(cookie);
    }
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
  // printf("cookiesForRequest: %s \n",request.asString().c_str());
  string domain(request.server());
  // find a cookie in the jar that corresponds to the requested domain...
  string tmp("Cookie: ");
  bool needSep(false);
  const static string sep(";");
  std::vector<Cookie *>::const_iterator it(m_cookies.begin());
  for (; it != m_cookies.end(); ++it)
  {
    Cookie *c(*it);
    if (c->name() == DOMAIN_STR)
    {
      continue;
    }

    if (c->matchesDomain(domain)) {
      if (not c->path().empty())
      {
        // check to see if the requested path is a sub dir of path
        string value = c->path();
        string file = request.fileName();
        size_t pos = file.find(value);
        if (pos != 0)
        {
          continue;
        }
      }
      // do not send secure cookies to unsecure domains
      if (c->secure() and request.protocol() != URI::HTTPS_PROTOCOL)
      {
        continue;
      }

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
  // printf("Return: %s\n", headers.c_str());
}

std::string CookieJar::topLevel(const std::string & sub)
{
  // given foo.blahblah.com, return blahblah.com
  size_t pos = sub.rfind(".");
  if (pos == string::npos or pos == 0)
  {
    return sub;
  }
  pos = sub.rfind(".", pos-1);
  if (pos == string::npos)
  {
    return sub;
  }
  return sub.substr(pos+1, sub.length()-pos-1);
}

bool CookieJar::acceptCookies(const std::string & domain) const
{

  // first get the top level domain of "domain".
  string top(topLevel(domain));
  AcceptedDomainMap::const_iterator topIt(m_acceptedDomains.find(top));
  if (topIt != m_acceptedDomains.end())
  {
    return true;
  }
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
