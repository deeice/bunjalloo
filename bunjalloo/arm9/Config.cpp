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
#include "Config.h"
#include "CookieJar.h"
#include "File.h"
#include "Language.h"
#include "HtmlConstants.h"
#include "HtmlElement.h"
#include "URI.h"

static const char * s_datadir = DATADIR;
static const char s_configFile[] = "config.ini";
static const char s_templateName[] = "config-example.txt";
static const char COOKIE_TEMPLATE[] = "ckallow-example.txt";
static const char USER_DIR[] = "/"DATADIR"/user";

const char Config::PROXY_STR[] = "proxy";
const char Config::FONT_STR[] = "font";
const char Config::COOKIE_STR[] = "cookiefile";
const char Config::CERT_FILE[] = "cafile";
const char Config::SEARCHFILE_STR[] = "searchfile";
const char Config::MAX_CONNECT[] = "timeout";
const char Config::USECACHE[] = "usecache";
const char Config::CLEARCACHE[] = "clearcache";
const char Config::DOWNLOAD[] = "download";
const char LANG_STR[] = "language";
using namespace std;

void Config::reload()
{
  string cfgFilename;
  cfgFilename += DATADIR;
  cfgFilename += "/";
  cfgFilename += s_configFile;

  if (nds::File::exists(cfgFilename.c_str()) == nds::File::F_NONE)
  {
    // write default config file
    string cfgTemplate(s_datadir);
    cfgTemplate += "/docs/";
    cfgTemplate += s_templateName;
    copyTemplate(cfgTemplate.c_str(), cfgFilename.c_str());
  }

  if (nds::File::exists(USER_DIR) == nds::File::F_NONE)
  {
    nds::File::mkdir(USER_DIR);
  }

  parseFile(cfgFilename.c_str());


  handleCookies();
}

void Config::callback(const std::string & first, const std::string & second)
{
  if (first == FONT_STR
      or first == COOKIE_STR
      or first == SEARCHFILE_STR
      or first == CERT_FILE)
  {
    string value;
    configPathMember(second, value);
    m_resources[first] = value;
  }
  else if (first == LANG_STR)
  {
    Language::instance().setLanguage(second);
    m_resources[first] = second;
  }
  else
  {
    m_resources[first] = second;
  }
}

void Config::handleCookies() const
{
  // configure the cookie list, read each line in the m_cookieList file and
  // add it as an allowed one to CookieJar
  nds::File cookieList;
  string cookieFile;
  resource(COOKIE_STR, cookieFile);
  if (nds::File::exists(cookieFile.c_str()) == nds::File::F_NONE)
  {
    string cookietemp(DATADIR);
    cookietemp += "/docs/";
    cookietemp += COOKIE_TEMPLATE;
    copyTemplate(cookietemp.c_str(), cookieFile.c_str());
  }
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
      m_cookieJar.setAcceptCookies(uri.server());
    }
    cookieList.close();
  }
}


Config::Config(CookieJar & cookieJar):
    m_cookieJar(cookieJar)
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


void Config::copyTemplate(const char * src, const char * dst)
{
  // copy a template file to the new file, stripping comments off
  nds::File srcFile;
  srcFile.open(src, "r");
  if (srcFile.is_open())
  {
    vector<string> lines;
    srcFile.readlines(lines);
    srcFile.close();

    nds::File dstFile;
    dstFile.open(dst, "w");
    for (vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it)
    {
      string & line(*it);
      stripWhitespace(line);
      if (not line.empty() and line[0] != '#')
      {
        dstFile.write(line.c_str());
        // Linux users know, Windows users will cry.
        dstFile.write("\r\n");
      }
    }
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

void Config::updateConfig(const std::string & first, const std::string & second,
    std::vector<std::string> & lines)
{
  bool replaced(false);
  callback(first, second);
  for (std::vector<std::string>::iterator it(lines.begin());
      it != lines.end(); ++it)
  {
    string & line(*it);
    ParameterSet set(line);
    if (set.hasParameter(first))
    {
      replaced = true;
      bool win(false);
      if (line[line.length()-1] == '\r')
      {
        win = true;
      }
      line = first + "=" + second;
      if (win)
        line += "\r";
      break;
    }
  }
  // append the line if it was not already in the file.
  if (not replaced)
  {
    string line(first);
    line += "=";
    line += second;
    string & firstLine(lines.front());
    if (firstLine[firstLine.length()-1] == '\r')
    {
      line += '\r';
    }
    lines.push_back(line);

  }
}

void Config::postConfiguration(const std::string & postedUrl)
{
  // printf("Post Config %s\n", postedUrl.c_str());
  // split on "&" and parse parameter pairs
  ParameterSet set(postedUrl, '&');
  const KeyValueMap & kv(set.keyValueMap());
  string cfgFilename;
  cfgFilename += DATADIR;
  cfgFilename += "/";
  cfgFilename += s_configFile;

  nds::File file;
  file.open(cfgFilename.c_str(), "r");
  vector<string> lines;
  file.readlines(lines);
  file.close();
  for (KeyValueMap::const_iterator it(kv.begin()); it != kv.end(); ++it)
  {
    // update the config.ini file, without breaking things
    updateConfig(it->first, it->second, lines);
  }
  file.open(cfgFilename.c_str(), "w");
  for (std::vector<std::string>::iterator it(lines.begin());
      it != lines.end(); ++it)
  {
    file.write(it->c_str(), it->length());
    file.write("\n");
  }
}
