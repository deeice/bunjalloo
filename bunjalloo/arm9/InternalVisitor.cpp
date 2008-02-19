#include "InternalVisitor.h"
#include "HtmlAnchorElement.h"

InternalVisitor::InternalVisitor(const std::string & name)
  : m_index(-1), m_found(false)
{
  m_name = string2unicode(name);
}

bool InternalVisitor::visit(HtmlAnchorElement & element)
{
  if (not m_found)
  {
    m_index++;
    const UnicodeString & name (element.attribute("name"));
    const UnicodeString & id (element.attribute("id"));
    if (id == m_name or name == m_name)
    {
      m_found = true;
    }
  }
  return true;
}
