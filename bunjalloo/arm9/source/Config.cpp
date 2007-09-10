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
#include "Config.h"
#include "ControllerI.h"
#include "CookieJar.h"
#include "Document.h"
#include "File.h"
#include "HtmlConstants.h"
#include "HtmlElement.h"
#include "ParameterSet.h"
#include "URI.h"
const std::string Config::s_configFile("/"DATADIR"/config.ini");

static const char PROXY_STR[] = "proxy";
static const char FONT_STR[] = "font";
static const char COOKIE_STR[] = "cookiefile";
static const char MAX_CONNECT[] = "timeout";
static const char TOOLBAR_TIME[] = "toolbar";
using namespace std;

std::string Config::font() const
{
  return m_font;
}

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
  if ( set.hasParameter(PROXY_STR) ) 
  {
    set.parameter(PROXY_STR, m_proxy);
  }
  if (set.hasParameter(FONT_STR))
  {
    string value;
    set.parameter(FONT_STR, value);
    configPathMember(value, m_font);
  }
  if (set.hasParameter(COOKIE_STR))
  {
    string value;
    set.parameter(COOKIE_STR, value);
    configPathMember(value, m_cookieList);
  }

  if (set.hasParameter(MAX_CONNECT))
  {
    string value;
    set.parameter(MAX_CONNECT, value);
    m_maxConnect = strtoul(value.c_str(), 0, 0);
  }

  if (set.hasParameter(TOOLBAR_TIME))
  {
    string value;
    set.parameter(TOOLBAR_TIME, value);
    m_toolbarTime = strtoul(value.c_str(), 0, 0);
  }
}

void Config::handleCookies() const
{
  // configure the cookie list, read each line in the m_cookieList file and
  // add it as an allowed one to CookieJar
  CookieJar * cookieJar(m_document.cookieJar());
  nds::File cookieList;
  cookieList.open(m_cookieList.c_str());
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
      cookieJar->setAcceptCookies(line);
    }
    cookieList.close();
  }
}


Config::Config(Document & doc)://, ControllerI & controller):
    m_document(doc),
    //m_controller(controller),
    m_font("font"),
    m_cookieList("ckallow.lst"),
    m_proxy("")
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

void Config::postConfiguration(const std::string & encodedString)
{
  ParameterSet set(encodedString, '&');
  parseLine(set);
}

std::string Config::proxy() const
{
  return m_proxy;
}

bool Config::useProxy() const
{
  return not m_proxy.empty();
}

unsigned int Config::maxConnections() const
{
  return m_maxConnect;
}

unsigned int Config::toolbarTimer() const
{
  return m_toolbarTime;
}
