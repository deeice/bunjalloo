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
#include "Rectangle.h"
#include "HtmlElement.h"
#include "HtmlOptionElement.h"
#include "TextArea.h"


Select::Select(HtmlElement * select):
  FormControl(select)
{}

void Select::addOption(const HtmlElement * option, const TextArea* textArea)
{
  if (option->hasChildren())
  {
    if (option->firstChild()->isa("#TEXT"))
    {
      // option has name - the text - and a value (if set)
      m_options.push_back(option);
      int size = textArea->textSize(option->firstChild()->text());
      if ( size > m_size->w)
      {
        if (size > MAX_SIZE)
          size = MAX_SIZE;
        m_size->w = size;
      }
    }
  }
}

void Select::draw(TextArea * textArea)
{
  /** Broken by BWT.
  FormControl::draw(textArea);
  ElementVector::const_iterator it(m_options.begin());
  for (; it != m_options.end(); ++it)
  {
    // print the option
    const HtmlOptionElement * option( (HtmlOptionElement*)(*it));
    if ( option->selected() )
    {
      // test setting the value attribute!
      textArea->printu(option->firstChild()->text());
      break;
    }
  }
  */
}
