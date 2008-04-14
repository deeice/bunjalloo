#include "NodeDumper.h"
#include "HtmlElement.h"
#include "HtmlAnchorElement.h"
#include "File.h"

NodeDumper::NodeDumper(const std::string & name):
  m_file(*(new nds::File))
{
  m_file.open(name.c_str(), "w");
}

NodeDumper::~NodeDumper()
{
  delete &m_file;
}

void NodeDumper::addAttribute(HtmlElement & element, const std::string & name)
{
  if (not m_file.is_open()) return;

  const UnicodeString & attr(element.attribute(name));
  if (not attr.empty())
  {
    m_file.write(name.c_str());
    m_file.write("=\"");
    m_file.write(unicode2string( attr, true ).c_str());
    m_file.write("\" ");
  }
}


bool NodeDumper::visit(HtmlAnchorElement & element)
{
  if (m_file.is_open())
  {
    // scoop out the middle?
  }
  return true;
}

void NodeDumper::begin(HtmlAnchorElement & element)
{
  if (m_file.is_open())
  {
    m_file.write("<A ");
    addAttribute(element, "href");
    addAttribute(element, "name");
    m_file.write(">");
  }
}

void NodeDumper::end(HtmlAnchorElement & element)
{
  if (m_file.is_open())
  {
    m_file.write("</A>");
  }
}

bool NodeDumper::visit(HtmlElement & element)
{
  return true;
}

void NodeDumper::begin(HtmlElement & element)
{
  if (m_file.is_open())
  {
    m_file.write("<");
    m_file.write(element.tagName().c_str());
    m_file.write(">");
  }
}

void NodeDumper::end(HtmlElement & element)
{
  if (m_file.is_open())
  {
    m_file.write("</");
    m_file.write(element.tagName().c_str());
    m_file.write(">");
  }
}

