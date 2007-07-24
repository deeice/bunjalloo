#include "Config.h"
#include "ControllerI.h"
#include "CookieJar.h"
#include "Document.h"
#include "File.h"
#include "HtmlElement.h"
#include "ParameterSet.h"
#include "URI.h"
const std::string Config::s_configFile("file:///"DATADIR"/config.html");

static const char PROXY_STR[] = "proxy";
static const char USE_PROXY_STR[] = "useProxy";
using namespace std;

Config & Config::instance()
{
  static Config s_instance;
  return s_instance;
}

std::string Config::font() const
{
  return m_font;
}

void Config::initialise(Document * doc, ControllerI * controller)
{
  m_document = doc;
  m_controller = controller;
  m_document->registerView(this);
  reload();
}

void Config::reload()
{
  m_reload = true;
  m_controller->doUri(s_configFile);
  // now configure the cookie list
  // read each line in the m_cookieList file and add it as an allowed one to CookieJar
  handleCookies();
}

void Config::handleCookies() const
{
  CookieJar * cookieJar(m_document->cookieJar());
  nds::File cookieList;
  cookieList.open(m_cookieList.c_str());
  if (cookieList.is_open())
  {
    // read each line
    int size = cookieList.size();
    char * data = new char[size+2];
    cookieList.read(data);
    int startOfLine = 0;
    for (int i = 0; i < size; ++i)
    {
      if (data[i] == '\n')
      {
        string domain(&data[startOfLine], i-startOfLine);
        cookieJar->setAcceptCookies(domain);
        startOfLine = i+1;
      }
    }
    delete [] data;
    cookieList.close();
  }
}

void Config::notify()
{
  if (m_document->status() == Document::LOADED)
  {
    if (m_reload)
    {
      configMember("font", m_font);
      configMember("li", m_cookieList);
      m_reload = false;
    }
  }
}

Config::Config():
    m_document(0),
    m_controller(0),
    m_reload(false),
    m_font("fonts/vera"),
    m_cookieList("cfg/ckallow.lst"),
    m_proxy(""),
    m_useProxy(false)
{
}

Config::~Config()
{
}

void Config::configMember(const std::string & tag, std::string & member)
{
  const HtmlElement * root = m_document->rootNode();
  const HtmlElement * body = root->lastChild();
  if (body->isa("body"))
  {
    const ElementList & children = body->children();
    ElementList::const_iterator element = find_if(children.begin(), children.end(), 
        bind2nd( mem_fun(&HtmlElement::isa_ptr), &tag)
        );
    if (element != children.end())
    {
      UnicodeString tmp;
      tmp = (*element)->attribute("id"); 
      member.clear();
      for (unsigned int i = 0; i < tmp.length(); ++i) {
        member += tmp[i];
      }
      if (member[0] != '/')
      {
        member = "/"+member;
        member = DATADIR+member;
      }
      // else is absolute path.
    }
  }
}

void Config::postConfiguration(const std::string & encodedString)
{
  ParameterSet set(encodedString, '&');
  if ( set.hasParameter(PROXY_STR) ) 
  {
    set.parameter(PROXY_STR, m_proxy);
    string useProxy;
    set.parameter(USE_PROXY_STR, useProxy);
    if (useProxy == "on")
    {
      m_useProxy = true;
    }
  }
}
