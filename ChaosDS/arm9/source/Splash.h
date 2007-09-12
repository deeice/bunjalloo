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
#ifndef Splash_h_seen
#define Splash_h_seen
#include "TouchScreen.h"

class Splash: public TouchScreen
{
  public:
    Splash();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_animationPalette;
    bool m_menuOn;
    int m_hilightItem;

    static void optionCb(void * arg);
    static void startCb(void * arg);
    static void showMenuCb(void * arg);

    // keypad handling
    void left();
    void right();
    void a();
    void start();

    void selectItem(int item);
};
#endif
