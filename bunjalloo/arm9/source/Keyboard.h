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
#ifndef Keyboard_h_seen
#define Keyboard_h_seen
class TextArea;
class Key;

namespace nds{
class Sprite;
}
#include <string>
#include <vector>

/** Show a virtual keyboard. Accepts touch input and keypad input. */
class Keyboard
{
  public:

    /** Construct a keyboard for the given TextArea.
     * @param textArea the thing that draws text.
     */
    Keyboard(TextArea & textArea);

    /** Destructor. */
    ~Keyboard();

    /** Set the keyboard visible or not.
     * @param visible true to make the keyboard visible, false otherwise.
     */
    void setVisible(bool visible=true);

    /** Get the current visibility.
     * @return true if visible false otherwise
     */
    bool visible() const;
    /** Call this each frame to handle keyboard input.*/
    void handleInput();

    /** Get the result of the last string entered (after Enter is "pressed").
     * @return the last string entered.
     */
    std::string result() const;

  private:
    int m_shift;
    TextArea & m_textArea;
    std::vector<Key*> m_keys;
    nds::Sprite * m_cursor;
    bool m_visible;
    bool m_redraw;
    int m_ticks;
    std::string m_result;

    void initKeys();
    void initCursor();
    void handleButtons(unsigned short keys);
    void checkPress(int x, int y);

    void doPress(Key * key);
    void setShift(int shift);
    void checkRedraw();
    void undrawResult();
    void deleteLetter();
    void drawResult();
    void clearResult();
    std::string selected();
};
#endif
