/*
  Copyright (C) 2008 Richard Quirk

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
#ifndef EditPopup_h_seen
#define EditPopup_h_seen

#include "PopupMenu.h"
class HtmlElement;
class View;
class EditPopup: public PopupMenu
{
  public:
    EditPopup(View * parent);
    void setElement(HtmlElement * el);
    UnicodeString details() const;
    void postEdit(const UnicodeString & val);

  private:
    View * m_parent;
    HtmlElement * m_element;
    static void editCallback(void* self);
    static void deleteCallback(void* self);
    void delElement();
    void editElement();
};
#endif
