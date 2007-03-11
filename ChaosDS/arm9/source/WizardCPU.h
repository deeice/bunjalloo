#ifndef WizardCPU_h_seen
#define WizardCPU_h_seen

class Wizard;

#include "Computer.h"
class WizardCPU : public Computer
{

  public:
    WizardCPU(Wizard & theWizard);

    enum Cast_t{
      DISBELIEVE,
      CREATURE
    };
    void aiCast(int spellType);
    void doAiSpell();
    void doAiMovement();
    bool hasTargetSquare() const;
    void endMove();

  private:
    bool m_targetSquareFound;
    int m_tableIndex;
    Wizard & m_wizard;
    int m_targetCount;
    bool m_hasMoved;
    bool m_moveTableCreated;

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
    void createAllEnemiesTable();

    void setupMove();
    void flyingMove(int type);

};
#endif
