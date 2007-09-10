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
#ifndef LinkListener_h_seen
#define LinkListener_h_seen

class Link;
/** Classes that want to register interest in link clicking should inherit from this interface. */
class LinkListener
{
  public:
    virtual ~LinkListener() {}

    /** Called when a Link is clicked somehwere.
     * @param link the link that has been clicked.
     */
    virtual void linkClicked(Link * link) = 0;

};
#endif
