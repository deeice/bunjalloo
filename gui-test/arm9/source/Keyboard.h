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
#ifndef Keyboard_h_seen
#define Keyboard_h_seen

#include "Component.h"
#include "TextListener.h"
#include "UnicodeString.h"

class Button;
class TextArea;
/** Show a virtual keyboard. Accepts touch input and keypad input. */
class Keyboard : public Component, public TextListener
{
  public:

    /** Construct a keyboard for the given TextArea.
     * @param textArea the thing that draws text.
     */
    Keyboard();

    /** Call this each frame to handle keyboard input.*/
    void handleInput();

    /** Get the result of the last string entered (after Enter is "pressed").
     * @return the last string entered.
     */
    UnicodeString result() const;

    virtual void paint(const nds::Rectangle & clip);
    virtual void editText(TextEntryI * entry);

  private:
    int m_shift;
    TextArea * m_textArea;
    std::vector<Button*> m_keys;
    UnicodeString m_result;

};
#endif
