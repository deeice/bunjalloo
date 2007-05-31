#ifndef Movement_h_seen
#define Movement_h_seen

#include "ArenaTouchScreen.h"

class Movement : public ArenaTouchScreen
{
  public:
    //!@param start true if this is the first time called. false if returning from examine screen.
    Movement(bool start=true);
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();

    //! Is the current movement of a flying creature.
    bool isFlying() const;

    //! Set the amount the selected thing can move.
    void setMovementAllowance(int ma);

    //! @return the movement allowance for the selected creature.
    inline int movementAllowance() const
    {
      return m_movementAllowance;
    }

    //! Press A to select the creature.
    virtual void execute();
    //! Press B to cancel the selection.
    void cancel();

    virtual void examine();

    //! Either cancel or end turn.
    virtual void next();

    //! @return the currently selected creature.
    inline int selectedCreature() const
    {
      return m_selectedCreature;
    }
    //! @return the range attack value for the currently selected creature.
    inline int rangeAttack() const
    {
      return m_rangeAttack;
    }
    //!@return true if it is engaged.
    inline bool isEngaged() const
    {
      return m_engagedFlag;
    }

  private:
    unsigned char m_selectedCreature;
    unsigned char m_rangeAttack;
    unsigned char m_movementAllowance;
    unsigned char m_isFlying;
    unsigned char m_engagedFlag;
    unsigned char m_creatureId;
    unsigned char m_rangeAttackVal;
    unsigned char m_wizardMovementAllowance;
    unsigned char m_attacker;
    unsigned char m_highlightItem;
    bool m_start;
    void startMovementRound();
    void start();
    void y();
    void end();
    void selectCreature();
    void moveFlyingCreature();
    void moveWalkingCreature();
    void moveCreature(int);
    void doRangeAttack();
    void checkEngaged(int index);
    void doSuccessfulMove(int distanceMoved);
    void makeAttack();
    void setEngagedToEnemy();
    void doAttackAnim(int index);
    void removeCreature(int creature);
};
#endif
