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
#ifndef ViewRender_h_seen
#define ViewRender_h_seen

class View;
class RichTextArea;
class HtmlElement;
class FormRadio;
#include <map>
#include "UnicodeString.h"

class ViewRender
{
  public:
    ViewRender(View * self);
    void render();

    // ElementList selectedRadioButtons() const;

  private:
    View * m_self;
    // keep track of the last RichTextArea component added to the ScrollPane
    RichTextArea * m_textArea;

    const HtmlElement * m_lastElement;
    typedef std::map<UnicodeString, FormRadio*> FormGroupMap;
    FormGroupMap m_radioGroup;

    RichTextArea * textArea();
    void walkTree(const HtmlElement * element);
    bool applyFormat(const HtmlElement * element);
    void preFormat(const HtmlElement * element);
    void postFormat(const HtmlElement * element);
    void setBgColor(const HtmlElement * body);
    void renderSelect(const HtmlElement * body);
    void doImage(const UnicodeString & unicode,
        const UnicodeString & src);
    void doTitle(const HtmlElement * title);
    void renderInput(const HtmlElement * inputElement);
    void renderTextArea(const HtmlElement * inputElement);
    void clearRadioGroups();

};
#endif
