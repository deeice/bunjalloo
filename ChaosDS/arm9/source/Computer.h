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
#ifndef Computer_h_seen
#define Computer_h_seen

class Computer
{

  public:
    //! Virtual destructor for base interface.
    virtual ~Computer() {}

    //! @return true if the computer has found a target square.
    virtual bool hasTargetSquare() const = 0;
    //! Set the target square value.
    virtual void setHasTargetSquare(bool) = 0;
    //! Make the computer cast the given spell id.
    virtual void aiCast(int spellType) = 0;
    //! start the computer spell casting.
    virtual void doAiSpell() = 0;
    //! start the computer movement.
    virtual void doAiMovement() = 0;

};

#endif
