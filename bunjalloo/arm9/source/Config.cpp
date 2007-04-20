#include "Config.h"
#include "Document.h"
#include "ControllerI.h"
#include "HtmlElement.h"
const std::string Config::s_configFile("file:///"DATADIR"/config.html");
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
}

void Config::notify()
{
  if (m_document->status() == Document::LOADED) {
    if (m_reload)
    {
      const HtmlElement * root = m_document->rootNode();
      const HtmlElement * body = root->lastChild();
      if (body->isa("body"))
      {
        const ElementList & children = body->children();
        string fonttxt("font");
        ElementList::const_iterator font = find_if(children.begin(), children.end(), 
            bind2nd( mem_fun(&HtmlElement::isa_ptr), &fonttxt)
            );
        if (font != children.end())
        {
          UnicodeString tmp;
          tmp = (*font)->attribute("id"); 
          m_font.clear();
          for (unsigned int i = 0; i < tmp.length();++i)
            m_font += tmp[i];
          if (m_font[0] != '/')
          {
            m_font = "/"+m_font;
          }
          m_font = DATADIR+m_font;
        }
      }
      m_reload = false;
    }
  }
}

Config::Config():
    m_document(0),
    m_controller(0),
    m_reload(false)
{
}

Config::~Config()
{
}
