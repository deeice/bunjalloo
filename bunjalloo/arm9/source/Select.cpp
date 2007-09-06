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
#include "Select.h"
#include "HtmlElement.h"
#include "HtmlOptionElement.h"
#include "HtmlConstants.h"


Select::Select(HtmlElement * select):m_element(select)
{
  if (m_element->hasChildren())
  {
    bool haveSelected(false);
    const ElementList & theChildren = m_element->children();
    ElementList::const_iterator it(theChildren.begin());
    for (; it != theChildren.end(); ++it)
    {
      if ( (*it)->isa(HtmlConstants::OPTION_TAG) ) {
        const HtmlOptionElement * option((HtmlOptionElement*)*it);
        if (option->hasChildren() and option->firstChild()->isa("#TEXT"))
        {
          this->addItem(option->firstChild()->text());
          if (option->selected())
          {
            int itemCount = items();
            if (itemCount > 0)
            {
              setSelectedIndex(itemCount - 1);
              haveSelected = true;
            }
          }
        }
      }
    }
    if (not haveSelected)
    {
      setSelectedIndex(0);
    }
  }
}

void Select::pressed(ButtonI * button)
{
  ComboBox::pressed(button);
  const ElementList & theChildren = m_element->children();
  int selected = selectedIndex();
  ElementList::const_iterator it(theChildren.begin());
  for (int index = 0; it != theChildren.end(); ++it)
  {
    if ( (*it)->isa(HtmlConstants::OPTION_TAG) )
    {
      const HtmlOptionElement * option((HtmlOptionElement*)*it);
      if (index == selected)
      {
        m_element->setAttribute("value", option->attribute("value"));
        break;
      }
      index++;
    }
  }
}
