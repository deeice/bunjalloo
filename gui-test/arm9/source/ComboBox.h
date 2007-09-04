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
#ifndef ComboBox_h_seen
#define ComboBox_h_seen
#include "Component.h"
#include "UnicodeString.h"
#include "ButtonListener.h"

class ScrollPane;
class Button;
/** A ComboBox is an item that shows a button with the initial value, and a
 * drop down list with potential values. When the button is clicked, the menu
 * is shown.*/
class ComboBox: public Component, 
                private ButtonListener
{
  public:
    /** Create an empty ComboBox.*/
    ComboBox();

    /** End of the objects existence.*/
    //~ComboBox();
    
    /** Add an item to the list. If it is the first item, it is selected.
     * @param item the text to add.
     */
    void addItem(const UnicodeString & item);

    /** The main button has been pressed. This shows the drop down list of items.
     * @param button the button that was pressed.
     */
    void pressed(ButtonI * button);

    // implemented from Component.
    virtual bool touch(int x, int y);
    virtual void paint(const nds::Rectangle & clip);
    virtual void setLocation(unsigned int x, unsigned int y);
    virtual void setSize(unsigned int w, unsigned int h);
  private:
    int m_items;
    bool m_open;

    inline ScrollPane * scrollPane() {
      return (ScrollPane*)m_children.front();
    }
    inline Button * button() {
      return (Button*)m_children.back();
    }
    
};
#endif
