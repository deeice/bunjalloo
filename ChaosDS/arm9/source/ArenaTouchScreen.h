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
#ifndef ArenaTouchScreen_h_seen
#define ArenaTouchScreen_h_seen

#include "TouchScreen.h"
class ExamineSquare;
class ArenaTouchScreen: public TouchScreen
{

  public:
    ArenaTouchScreen();
    ~ArenaTouchScreen();

    /** Examine the current square.  */
    void examine();

    /** Execute action on the current square (selected twice) */
    virtual void execute() = 0;

    /** Next action button pressed. */
    virtual void next() = 0;

  private:
    static void examineCb(void * arg);
    static void exitCb(void * arg);
    static void arenaCb(void * arg);

    ArenaTouchScreen(const ArenaTouchScreen &);
    const ArenaTouchScreen & operator=(const ArenaTouchScreen &);

  protected:
    ExamineSquare * m_examineScreen;

};
#endif
