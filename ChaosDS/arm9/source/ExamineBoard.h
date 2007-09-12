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
#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ArenaTouchScreen.h"

class ExamineBoard: public ArenaTouchScreen
{
  public:
    /** Show the examine board screen.
     * @param start true if this is the first time calling. false if returning (e.g. after examine square)
     */
    ExamineBoard(bool start=true);
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();

    // from ArenaTouchScreen
    virtual void next();
    virtual void execute();

  private:
    bool m_start;

};
#endif
