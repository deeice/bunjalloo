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
#ifndef FormControl_h_seen
#define FormControl_h_seen

#include "UnicodeString.h"
#include "Rectangle.h"

class TextArea;
class ControllerI;
class HtmlElement;
class URI;

class FormControl
{
  public:
    static const int MAX_SIZE;
    static const int MIN_SIZE;
    FormControl(HtmlElement * element);
    virtual ~FormControl();
    virtual void draw(TextArea * gfx);

    enum InputType
    {
      KEYBOARD,
      ONE_CLICK,
      MENU
    };

    /** Does this control require keyboard input? .
     * @return true if it does. false if only clicks are needed.
     */
    virtual InputType inputType() const;

    /** Input a unicode string.
     * @param str the input value.
     */
    virtual void input(const UnicodeString & str);

    /** input a touch value after clicking.
     * @param str the input touch.
     */
    virtual void input(int x, int y, ControllerI & controller, URI & uri);

    void setHeight(int h);
    void setPosition(int x, int y);
    int width() const;
    bool hitTest(int x, int y) const;

  protected:
    nds::Rectangle * m_size;
    HtmlElement * m_element;
    std::string m_processedData;
};

#endif
