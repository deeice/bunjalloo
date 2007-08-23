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
#ifndef Casting_h_seen
#define Casting_h_seen

#include "ArenaTouchScreen.h"

class Casting: public ArenaTouchScreen
{
  public:
    /** initialise the object. 
     * @param start is it a start screen or returning.
     */
    Casting(bool start=true);

    //! get the current global chaos value
    static int worldChaos();
    //! set the global chaos value to 0.
    static void resetWorldChaos();
    //! set the spell success indicator to the given value.
    static void setSpellSuccess(bool success);
    /** Calculate if the spell succeeds, using the spell cast chance, illusion
     * flag and random factors.
     */
    static bool calculateSpellSuccess();
    //! @return true if the spell succeeds.
    static bool spellSuccess();
    //! Do the spell animation from the start index ot the target index.
    static void spellAnimation();
    //! Print the outcome of the spell cast.
    static void printSuccessStatus();
    
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();

    virtual void execute();
    virtual void next();
  private:
    static int s_worldChaos;
    static bool s_castSuccess;

    bool m_start;
    void startCastRound();
    void nextPlayerCast();
    
};
#endif
