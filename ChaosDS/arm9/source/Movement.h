#ifndef Movement_h_seen
#define Movement_h_seen

#include "ScreenI.h"

class Movement : public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t screenId() const;
    virtual void handleKeys();

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
    void startMovementRound();
};
#endif
