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
#include "Config.h"
#include "CookieJar.h"
#include "Document.h"
#include "File.h"
#include "HtmlConstants.h"
#include "HtmlElement.h"
#include "URI.h"
const std::string Config::s_configFile("/"DATADIR"/config.ini");

const char Config::PROXY_STR[] = "proxy";
const char Config::FONT_STR[] = "font";
const char Config::COOKIE_STR[] = "cookiefile";
const char Config::MAX_CONNECT[] = "timeout";
const char Config::TOOLBAR_TIME[] = "toolbar";
const char Config::USECACHE[] = "usecache";
const char Config::CLEARCACHE[] = "clearcache";
using namespace std;

void Config::reload()
{
  nds::File configFile;
  configFile.open(s_configFile.c_str());
  if (configFile.is_open())
  {
    vector<string> lines;
    configFile.readlines(lines);
    for (vector<string>::iterator it(lines.begin());
        it != lines.end();
        ++it)
    {
      string & line(*it);
      stripWhitespace(line);
      if (not line.empty() and line[0] != '#')
      {
        ParameterSet set(line);
        parseLine(set);
      }
    }
    configFile.close();
  }

  handleCookies();
}

void Config::parseLine(ParameterSet & set)
{
  const KeyValueMap& keyValueMap(set.keyValueMap());
  for (KeyValueMap::const_iterator it(keyValueMap.begin());
      it != keyValueMap.end();
      ++it)
  {
    if (it->first == FONT_STR or it->first == COOKIE_STR)
    {
      string value;
      configPathMember(it->second, value);
      m_resources[it->first] = value;
    }
    else
    {
      m_resources[it->first] = it->second;
    }
  }
}

void Config::handleCookies() const
{
  // configure the cookie list, read each line in the m_cookieList file and
  // add it as an allowed one to CookieJar
  CookieJar * cookieJar(m_document.cookieJar());
  nds::File cookieList;
  string cookieFile;
  resource(COOKIE_STR, cookieFile);
  cookieList.open(cookieFile.c_str());
  if (cookieList.is_open())
  {
    vector<string> lines;
    cookieList.readlines(lines);
    for (vector<string>::iterator it(lines.begin());
        it != lines.end();
        ++it)
    {
      string & line(*it);
      stripWhitespace(line);
      URI uri(line);
      cookieJar->setAcceptCookies(uri.server());
    }
    cookieList.close();
  }
}


Config::Config(Document & doc):
    m_document(doc)
{
  reload();
}

Config::~Config()
{
}

void Config::configPathMember(const std::string & value, std::string & member)
{
  member.clear();
  if (value[0] != '/')
  {
    // == "/DATADIR/value"
    member = "/"+value;
    member = DATADIR+member;
  }
  else
  {
    // is absolute path.
    member = value;
  }
}

bool Config::resource(const std::string & name, std::string & value) const
{
  KeyValueMap::const_iterator it(m_resources.find(name));
  if (it != m_resources.end())
  {
    value = it->second;
    return true;
  }
  return false;
}

bool Config::resource(const std::string & name, bool & value) const
{
  string valueString;
  if (resource(name, valueString))
  {
    value = valueString == "true" or valueString == "1";
    return true;
  }
  return false;
}

bool Config::resource(const std::string & name, int & value) const
{
  string valueString;
  if (resource(name, valueString))
  {
    value = strtol(valueString.c_str(), 0, 0);
    return true;
  }
  return false;
}

