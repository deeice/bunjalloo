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
#ifndef ToggleButton_h_seen
#define ToggleButton_h_seen

#include "ButtonI.h"
#include "Component.h"

/** A dual state button that toggles on and off as the user presses it.*/
class ToggleButton: public Component, public ButtonI
{
  public:
    /** Construct a new ToggleButton.*/
    ToggleButton();

    // from Component
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  protected:
    /** Concrete classes that inherit from ToggleButton should return the
     * graphics to display.
     * @return the graphics data, in "sprite base" tile format. The implementer
     * should return the correct data for selected/unselected state by checking
     * selected()
     */
    virtual const unsigned char *getGfxData() = 0;

    /** Concrete classes that inherit from ToggleButton should return the
     * palette for the displayed graphics.
     * @return the palette data that corresponds to the value returned by getGfxData()
     */
    virtual const unsigned short *getPaletteData() = 0;

  private:
};
#endif
