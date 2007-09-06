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

#include "ComboBox.h"

class HtmlElement;
/** A form select control. Shows a drop down list of items.
 */
class Select: public ComboBox
{
  public:
    Select(HtmlElement * select);

    /** Overridden from ComboBox::pressed() in order to capture selection
     * events and set the HtmlElement value.
     * @param button the menu button pressed.
     */
    virtual void pressed(ButtonI * button);

  private:
    HtmlElement * m_element;

};

#endif
