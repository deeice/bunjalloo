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
#ifndef CreatePlayers_h_seen
#define CreatePlayers_h_seen

#include "TouchScreen.h"
class CreatePlayers: public TouchScreen
{
  public:
    /** @param start flag to say if the screen is the first, or if it should
     * only redraw already existing wizard details. If true, then it will
     * create a new set of wizards.
     */
    CreatePlayers(bool start=true);

    virtual void show();
    virtual void animate();
    virtual void handleKeys();
    
  private:
    int m_hilightItem;
    int m_hilightWizard;
    bool m_start;

    static void incrPlayerCb(void * arg);
    static void decrPlayerCb(void * arg);
    static void changeIconCb(void * arg);
    static void changeIconColCb(void * arg);
    static void changeNameCb(void * arg);
    static void changeLevelCb(void * arg);
    static void resetPlayersCb(void * arg);
    static void startCb(void * arg);

    bool selectWizFromY();

    void selectItem(int);
    void deselectItem();
    void updatePlayers(void);

    void up();
    void down();
    void left();
    void right();
    void a();
    void l();
    void r();
    void start();
    
};
#endif
