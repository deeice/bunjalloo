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
#include <vector>
#include "ElementFactory.h"
#include "HtmlConstants.h"
#include "HtmlElement.h"
#include "HtmlParser.h"

using namespace std;

#include "Visitor.h"
IMPLEMENT_ACCEPT(HtmlElement);

const UnicodeString * HtmlElement::attributePtr(const std::string & name) const
{
  if (name == "id")
  {
    return &m_id;
  }
  if (name == "title")
  {
    return &m_title;
  }
  if (name == "lang")
  {
    return &m_lang;
  }
  if (name == "dir")
  {
    return &m_dir;
  }
  return 0;
}

UnicodeString HtmlElement::attribute(const std::string & name) const
{
   const UnicodeString * ptr = attributePtr(name);
   if (ptr)
   {
     return *ptr;
   }
   return UnicodeString();
}

void HtmlElement::setAttribute(const std::string & name, const UnicodeString & value)
{
   UnicodeString * ptr = const_cast<UnicodeString*>(attributePtr(name));
   if (ptr)
   {
     *ptr = value;
   }
}

void HtmlElement::append(HtmlElement * child)
{
  m_children.push_back(child);
  child->setParent(this);
}

void HtmlElement::remove(HtmlElement * child)
{
  ElementList::iterator forRemoval = find(m_children.begin(), m_children.end(), child);
  if (forRemoval != m_children.end())
  {
    m_children.erase(forRemoval);
  }
}

void HtmlElement::appendText(unsigned int value)
{
  if (m_children.size())
  {
    if (m_children.back()->isa(HtmlConstants::TEXT))
    {
      UnicodeString & text(m_children.back()->m_text);
      if (not ::isblank(value) or (::isblank(value) and not ::isblank(text[text.length()-1])))
      {
        m_children.back()->m_text += value;
      }
      return;
    }
  }
  // ignore spaces at the start.
  if (isWhitespace(value) or ::isblank(value))
    return;
  HtmlElement* textNode = new HtmlElement(HtmlConstants::TEXT);
  textNode->m_text = value;
  append(textNode);
}

HtmlElement::~HtmlElement()
{
  for_each(m_children.begin(), m_children.end(), ElementFactory::remove);
}

void HtmlElement::copyAttributes(HtmlElement * copyTo) const
{
  copyTo->m_id = m_id;
  copyTo->m_title = m_title;
  copyTo->m_lang = m_lang;
  copyTo->m_dir = m_dir;
}

HtmlElement * HtmlElement::clone() const
{
  HtmlElement * theClone = new HtmlElement(m_tagName);
  copyAttributes(theClone);
  return theClone;
}

#if 0
void HtmlElement::dump() const
{
  std::basic_string<wchar_t> dumper;
  UnicodeString::const_iterator it(m_text.begin());
  for (; it != m_text.end(); ++it)
  {
    dumper += *it;
  }

  wcout << dumper << endl;
}
#endif

void HtmlElement::removeAllChildren()
{
  m_children.clear();
}

const HtmlElement * HtmlElement::previousSibling(const HtmlElement * child)
{

  ElementList::const_iterator found = find(m_children.begin(), m_children.end(), child);
  if (found != m_children.end()) {
    --found;
    if (found != m_children.end()) {
      return *found;
    }
  }
  return 0;
}
const HtmlElement * HtmlElement::nextSibling(const HtmlElement * child)
{
  ElementList::const_iterator found = find(m_children.begin(), m_children.end(), child);
  if (found != m_children.end()) {
    ++found;
    if (found != m_children.end()) {
      return *found;
    }
  }
  return 0;
}

ElementList HtmlElement::elementsByTagName(const std::string & name) const
{
  string lowerName = name;
  transform(name.begin(), name.end(), lowerName.begin(), ::tolower);
  ElementList returnList;
  ElementList::const_iterator it(m_children.begin());
  for (; it != m_children.end(); ++it)
  {
    HtmlElement * element(*it);
    if (element->isa(name))
    {
      returnList.push_back(element);
    }
  }
  return returnList;
}

void HtmlElement::clearText() const
{
  UnicodeString().swap(m_text);
}

void HtmlElement::visitChildren(Visitor & visitor)
{
  for (ElementList::iterator it(m_children.begin()); it != m_children.end(); ++it)
  {
    (*it)->accept(visitor);
  }
}

