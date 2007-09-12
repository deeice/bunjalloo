/*
  Copyright (C) 2007 Richard Quirk

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
#ifndef Button_h_seen
#define Button_h_seen

#include "TextContainer.h"
#include "ButtonI.h"
#include "UnicodeString.h"

class TextArea;
class ButtonListener;

/** A press-style button with a textual label. */
class Button: public TextContainer, public ButtonI
{
  public:
    /** Creates a new empty Button. */
    Button();
    /** Creates a new Button with the given label. 
     * @param label the text label that is to be shown.
     */ 
    Button(const UnicodeString & label);

    /** Set whether to paint decoration - the shadow and highlight, plus
     * pressed/depressed colours - or not.
     * @param decorate flag that is used to set decoration.
     */
    void setDecoration(bool decorate=true);

    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  private:
    bool m_decoration;
};
#endif
