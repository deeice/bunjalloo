/*
  Copyright (C) 2008 Richard Quirk

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
#include "Document.h"
#include "EditPopup.h"
#include "HtmlElement.h"
#include "Language.h"
#include "NodeDumper.h"
#include "View.h"

EditPopup::EditPopup(View * parent):
  m_parent(parent)
{
  setVisible(false);
  addMenuItem(T("edsel"), editCallback, this);
  addMenuItem(T("delsel"), deleteCallback, this);
  updateLayout();
}

void EditPopup::setElement(HtmlElement * element)
{
  m_element = element;
}

void EditPopup::deleteCallback(void* self)
{
  ((EditPopup*)self)->delElement();
}

void EditPopup::editCallback(void* self)
{
  ((EditPopup*)self)->editElement();
}

void EditPopup::delElement()
{
  // Remove the element from the bookmark file.
  // This is *insane*.
  HtmlElement * p(m_element->parent());
  p->remove(m_element);
  // now dump the Bookmark file to disk
  {
    NodeDumper dumper(Config::BOOKMARK_FILE);
    HtmlElement * root((HtmlElement*)m_parent->document().rootNode());
    root->accept(dumper);
  }
  m_parent->bookmarkUrl();
}

void EditPopup::editElement()
{
  // edit the element name...
  m_parent->editBookmark();
}

UnicodeString EditPopup::details() const
{
  UnicodeString val(m_element->attribute("href"));
  val += '\n';
  val += m_element->firstChild()->text();
  return val;
}

void EditPopup::postEdit(const UnicodeString & val)
{
  using std::vector;
  using std::string;
  vector<string> tokens;
  tokenize(unicode2string(val, true), tokens, "\n");
  m_element->setAttribute("href", string2unicode(tokens[0]));
  UnicodeString & t(m_element->firstChild()->text());
  t = string2unicode(tokens[1]);
  for (size_t i = 2; i < tokens.size(); ++i)
  {
    t += string2unicode(tokens[i]);
  }
  NodeDumper dumper(Config::BOOKMARK_FILE);
  HtmlElement * root((HtmlElement*)m_parent->document().rootNode());
  root->accept(dumper);
}
