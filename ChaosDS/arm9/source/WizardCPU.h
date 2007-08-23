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
#ifndef WizardCPU_h_seen
#define WizardCPU_h_seen

class Wizard;
class Arena;

#include "Computer.h"
class WizardCPU : public Computer
{
  friend class Movement;

  public:
    //! Create a WizardCPU for a given wizard.
    WizardCPU(Wizard & theWizard);

    //! Cast types used for aiCast().
    enum Cast_t {
      DISBELIEVE,
      CREATURE,
      TREES,
      WALL,
      MAGIC_MISSILE,
      JUSTICE,
      RAISEDEAD,
      SUBVERSION,
      TURMOIL,
    };
    void aiCast(int spellType);
    void doAiSpell();
    void doAiMovement();
    bool hasTargetSquare() const;
    void setHasTargetSquare(bool);
    //! End the movement round for the computer.
    void endMove();
    //! Make the computer dismount, if it wants to.
    bool dismount() const;
    //! Set if the move table needs recreating or not.
    void setMoveTableCreated(bool created);

    //! Set if the computer has found a flying target.
    inline void setFlyingTargetFound(bool found) {
      m_flyingTargetFound = found;
    }

  private:
    bool m_targetSquareFound;
    int m_tableIndex;
    Wizard & m_wizard;
    int m_targetCount;
    bool m_hasMoved;
    bool m_moveTableCreated;
    bool m_flyingTargetFound;

    int strongestWizard(int attackerIndex);
    void resetPriorityTable();
    int createRangeTable(int target, int range);
    // set the target index to the furthest square in range
    void setFurthestInrange();
    void createTableEnemies();
    void createEnemyTableEntry(int wizardid);
    void createTableWizards(); 

    void aiCastCreature();
    void aiCastDisbelieve();
    void aiCastTreesCastles();
    void aiCastWall();
    void aiCastMagicMissile();
    void aiCastJustice();
    void aiCastSubversion();
    void aiCastRaiseDead();
    void aiCastTurmoil();

    void createAllEnemiesTable();
    bool shouldCastSleepBlind();

    void doThisMovement();
    void setupMove();
    void flyingMove(int type);
    int getPriorityVal(int index);
    void getSurroundingSquarePrios(int index, int strongest);
    int getBestRangeattack();
    void doFlyingMove();
    void setupCreatureMove();
    int getBestIndex();
    void addBestIndex(Arena & arena);


};
#endif
