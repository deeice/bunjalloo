/*
  Copyright (C) 2007,2008 Richard Quirk

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
#ifndef Stylus_h_seen
#define Stylus_h_seen
#include <vector>
class StylusListener;
/** Stylus idea inspired by Jeff Lait's Powder.
 * User places the pointer on the screen in an area. He then let's go.
 * If the touch and let go are in the same place (more or less) then it is a valid click.
 * If the user holds down for a while, then that is not a click - it's a hold.
 */
class Stylus
{

  public:
    /** Create the stylus object. */
    Stylus();
    /** End of its life.*/
    ~Stylus();

    enum TouchType
    {
      NOTHING,        //!< Stylus is floating about the ether
      UP,             //!< Stylus has just gone up
      DOWN,           //!< Stylus is down
    };
    /** Get the user touch type.*/
    inline TouchType touchType() const
    {
      return m_state;
    }

    /** if DOWN, then is it a repeat triggering down?.*/
    bool isRepeat() const
    {
      return m_repeat;
    }

    inline bool down() const
    {
      return touchType() == DOWN;
    }

    inline bool up() const
    {
      return touchType() == UP;
    }

    /** Get the start point of the event.
     * @param x the x position.
     * @param y the y position.
     */
    void startPoint(int & x, int & y) const;

    /** Get the last point of the touch event.
     * calls may return different results. This allows unsure user stylus
     * scraping.
     * @param x the x position.
     * @param y the y position.
     */
    void lastPoint(int & x, int & y) const;

    int startX() const;
    int startY() const;
    int lastX() const;
    int lastY() const;

    /** Update the stylus state.
     * @param touch is the stylus up, down, or down and repeated.
     * @param x the x pos
     * @param y the y pos
     */
    void update(TouchType touch, bool repeat, int x, int y);

    /** Notify the listener to stylus events. 
     * @param listener the interested client. 
     */
    void registerListener(StylusListener * listener);
    /** Forget that we are notifying this component. 
     * @param listener the client that is no longer interested. 
     */
    void unregisterListener(StylusListener * listener);

    static TouchType keysToTouchType(bool held, bool up);

    /** Global variables, I couldn't think of a better way to do this that
     * didn't involve passing a stylus pointer to everyone that wanted them */
    static void registerStylus(Stylus * stylus);
    static Stylus * instance();
    static void deleteInstance();

  private:
    static Stylus * s_stylus;
    int m_startX;
    int m_startY;
    int m_lastX;
    int m_lastY;
    bool m_repeat;
    TouchType m_state;
    typedef std::vector<StylusListener*> StylusListenerVector;
    StylusListenerVector m_listeners;

    void notifyDownFirst();
    void notifyDownRepeat();
    void notifyDown();
    void notifyUp();
};
#endif
