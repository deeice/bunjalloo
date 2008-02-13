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
#ifndef LinkHandler_h_seen
#define LinkHandler_h_seen

#include "ButtonListener.h"
#include "ComboBox.h"
class LinkListener;
class Link;
class Stylus;

/** Handle Link presses that are images so the user can decide to follow the
 * anchor or view the image.
 */
class LinkHandler: public ComboBox
{

  public:
    /** Create the instance.
     * @param parent the LinkListener that received the linkClicked() call.
     */
    LinkHandler(LinkListener * parent);

    ~LinkHandler();

    /** Set the Link to handle. It should be a img/anchor type Link.
     * @param link the Link to deal with.
     */
    void setLink(const Link * link);

    // reimplemented from ComboBox
    virtual void pressed(ButtonI * button);
    virtual void paint(const nds::Rectangle & clip);

    virtual bool stylusUp(const Stylus * stylus);
    virtual bool stylusDownFirst(const Stylus * stylus);
  private:
    LinkListener * m_parent;
    const Link * m_link;
};
#endif
