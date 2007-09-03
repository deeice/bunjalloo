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
#ifndef WidgetColors_h_seen
#define WidgetColors_h_seen


/** Wrapper class for general widget colors.*/
class WidgetColors
{
  public:
    //! Button forground.
    const static unsigned short BUTTON_FOREGROUND;
    //! Button pressed.
    const static unsigned short BUTTON_PRESSED;
    //! Button shadow.
    const static unsigned short BUTTON_SHADOW;
    //! Button highlight.
    const static unsigned short BUTTON_SHINE;
    //!ComboBox drop down list background.
    static const unsigned short COMBOBOX_DROP_DOWN;
    //!ComboBox main button.
    static const unsigned short COMBOBOX_FOREGROUND;
    //!ComboBox main button when the list is shown.
    static const unsigned short COMBOBOX_SELECTED;
    //!ScrollBar background.
    static const unsigned short SCROLLBAR_BACKGROUND;
    //!ScrollBar border.
    static const unsigned short SCROLLBAR_BORDER;
    //!ScrollBar arrow background.
    static const unsigned short SCROLLBAR_ARROW;
    //!ScrollBar arrow detail.
    static const unsigned short SCROLLBAR_ARROW_HEAD;
    //!ScrollBar handle when not active.
    static const unsigned short SCROLLBAR_HANDLE_NOT_HELD;
    //!ScrollBar handle when the user has it selected.
    static const unsigned short SCROLLBAR_HANDLE_HELD;
};
#endif
