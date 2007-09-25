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
#ifndef FormControl_h_seen
#define FormControl_h_seen

#include <string>
#include "Button.h"
class HtmlElement;
class ControllerI;
class URI;

class FormControl : public Button
{
  public:
    static const int MAX_SIZE;
    static const int MIN_SIZE;
    FormControl(const HtmlElement * element, const UnicodeString & text);

    /** input a touch value after clicking.
     * @param str the input touch.
     */
    virtual void input(ControllerI & controller, URI & uri);

  protected:
    const HtmlElement * m_element;
    std::string m_processedData;
};

#endif
