/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef VictoryScreen_h_seen
#define VictoryScreen_h_seen

#include "TouchScreen.h"

class VictoryScreen: public TouchScreen
{
  public:
    //!Types of possible end game.
    enum Victory_t {
      WIN,       //!< There was a winner.
      DRAW,      //!< No winner, game drawn.
      CONTINUE   //!< No human players, check continue state
    };
    //! Start the end game screen.
    VictoryScreen(Victory_t type);
    virtual void show();
    virtual void animate();
    virtual void handleKeys();

    /** Call this when only cpu players remain. Asks the player if they want to
     * continue. */
    static void endGameOption();
  private:
    Victory_t m_winOrDraw;
    int m_highlightItem;
    int m_cycleColour1;
    int m_cycleColour2;
    int m_cycleColour3;
    int m_cycleFrame;
    bool m_decided;

    static void exitCb(void * arg);
    static void continueYesCb(void * arg);
    static void continueNoCb(void * arg);

    void exit();

    void win();
    void draw();
    void continueGame();

    void continueKeys();
};
#endif
