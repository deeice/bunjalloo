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
static const char USE_PROXY_STR[] = "useProxy";
static const char FONT_STR[] = "font";
static const char COOKIE_STR[] = "cookiefile";
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
  if (set.hasParameter(USE_PROXY_STR))
  {
    string useProxy;
    set.parameter(USE_PROXY_STR, useProxy);
    m_useProxy = (useProxy == "on");
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
    m_proxy(""),
    m_useProxy(false)
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
