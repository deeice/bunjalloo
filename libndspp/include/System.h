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
#ifndef System_h_seen
#define System_h_seen

// tolua_begin
namespace nds
{
  class System
  {

    public:
      /**
       * When the lid is closed, this function sets up a lid open interrupt,
       * powers down and sleeps. When the lid is opened again, the power is
       * restored and the program continues.
       * <p>
       * On the arm7 all interrupts are turned off except IRQ_LID. On the arm9
       * no changes are made to the IRQ flags. Since the arm7 has deactivated
       * VBLANK, the arm9 sleeps until the the arm7 turns IRQ_VBLANK on again -
       * this occurrs when the arm7 is woken by the LID being opened.
       */
      static void checkSleep();

      /** Get the system name. */
      static const char * uname();

  };
}
// tolua_end
#endif
