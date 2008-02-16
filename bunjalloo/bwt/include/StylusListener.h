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
#ifndef StylusListener_h_seen
#define StylusListener_h_seen

class Stylus;
/** Class. */
class StylusListener
{
  public:
    virtual ~StylusListener() {}

    /** Stylus up event.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusUp(const Stylus * stylus) = 0;

    /** Stylus is pressed for the first time since resetting.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDownFirst(const Stylus * stylus) = 0;

    /** Stylus is pressed down and has triggered repeat.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDownRepeat(const Stylus * stylus) = 0;

    /** Stylus is pressed down, it is just down, neither the first time nor repeating.
     * @param stylus pointer to the event.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool stylusDown(const Stylus * stylus) = 0;

};

#endif
