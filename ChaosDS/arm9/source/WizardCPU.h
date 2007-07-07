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
