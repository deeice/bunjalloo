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
#ifndef Select_h_seen
#define Select_h_seen

#include <vector>
#include "FormControl.h"

class HtmlElement;

/** A form select control. Shows a drop down list of items.
 */
class Select: public FormControl
{
  public:
    Select(HtmlElement * select);
    /** Add an option to the selection.
     * @param option the option to add.
     * @param textArea the text area - needed to get the font metrics for the option text.
     */
    void addOption(const HtmlElement * option, const TextArea * textArea);

    virtual void draw(TextArea * gfx);

  private:

    typedef std::vector<const HtmlElement*> ElementVector;
    ElementVector m_options;

};

#endif
