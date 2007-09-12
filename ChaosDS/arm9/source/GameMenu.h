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
#ifndef GameMenu_h_seen
#define GameMenu_h_seen

#include "TouchScreen.h"

class GameMenu: public TouchScreen
{
  public:
    GameMenu();
    //! Continue the game, corresponds to option 3 on the menu.
    static void continueGame();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_hilightItem;

    static void continueCb(void * arg);
    static void examineCb(void * arg);
    static void selectSpellCb(void * arg);
    void deselectItem(int item);
    inline void deselectItem() { deselectItem(m_hilightItem); };
    void selectItem();
    void up(); 
    void down();
    void a();
};
#endif
