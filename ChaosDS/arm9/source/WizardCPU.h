#ifndef WizardCPU_h_seen
#define WizardCPU_h_seen

class Wizard;

#include "Computer.h"
class WizardCPU : public Computer
{

  public:
    WizardCPU(Wizard & theWizard);

    void aiCastCreature();
    void doAiSpell();
    void doAiMovement();
  private:
    bool m_targetSquareFound;
    int m_tableIndex;
    Wizard & m_wizard;
    int m_targetCount;

    int strongestWizard(int attackerIndex);
    void resetPriorityTable();
    int createRangeTable(int target, int range);
    // set the target index to the furthest square in range
    void setFurthestInrange();
    void createTableEnemies();
    void createEnemyTableEntry(int wizardid);
    void createTableWizards(); 

};
#endif
