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
#ifndef Wifi7_h_seen
#define Wifi7_h_seen

#ifndef ARM7
#error Wifi7 is only available on the ARM7
#endif

namespace nds {
  /*! @brief Wifi7 is a helper class for the arm7 dswifi code.
   *
   *  This class initialises the DS wifi hardware and has a main loop that
   *  handles connections and disconnections.
   */
  class Wifi7
  {
    public:
      //! creates an instance of the class and initialises the wifi hardware.
      Wifi7();
      
      /*! @brief an infinite loop that handles connect, disconnect and reconnect.
       * Calls the software interrrupt vblank wait in between to use less power.
       */
      void mainLoop();
    private:
      //! is the hardware enabled or not
      bool m_enabled;
      //! initialise the hardware
      void initialise();
  };
};

#endif
