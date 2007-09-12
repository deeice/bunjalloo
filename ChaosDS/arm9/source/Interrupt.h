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
#ifndef Interrupt_h_seen
#define Interrupt_h_seen
namespace Interrupt
{
  //! handle the vblank interrupt.
  void processVblank();

  //! Enable the interrupt - really only enables the animation handling.
  void enable();
  //! Disable the interrupt - really only disables the animation handling.
  void disable();
};
#endif
