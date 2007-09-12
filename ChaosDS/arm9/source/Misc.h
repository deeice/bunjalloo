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
#ifndef Misc_h_seen
#define Misc_h_seen
// Hurray! Misc functions

namespace Misc
{
  /*! @brief get a random number
   * @param maximum the range of the random number is 0-(maximum-1)
   * @return a random number 0-@arg maximum -1
   */
  unsigned int rand(unsigned int maximum);
  /*! @brief set the random number seed
   * @param s the seed to use
   */
  void seedRand(unsigned int s);
  /*! @brief churn the random number once */
  void churnRand();

  /*! @brief trim white space from a string in place.
   * @param str the string to trim
   * @return pointer to the passed in character array
   */
  char * trim(char * str);
  /*! @brief Order a table of values by the priority in place.  Based on code
   * at c64c.
   * @param count the number of element pairs in the table. i.e a table of 40
   * bytes would have 20 pairs.
   * @param table the table to order
   */
  void orderTable(int count, unsigned char * table);

  /** Wait 1 vblank then check for a key press.
   * @return true if a key is pressed, false otherwise.
   */
  bool keypressWait();
  /** Wait for the player to let go of all keys. Spins (low power spin) until no key is pressed.
   */
  void waitForLetgo();

  /** Delay for the given number of frames. Optionally calls the main loop.
   * @param time the number of frames to wait (1/60th of a second per frame)
   * @param callMainLoop true if the main loop should be called while frozen. 
   * Use this to keep animating the screen during the delay.
   */
  void delay(int time, bool callMainLoop=true);
  
  
  /** Confirm an action. This call blocks until A or B is pressed.
   * @return true if A is pressed, false if B is pressed.
   */
  bool confirm();

}
#endif
