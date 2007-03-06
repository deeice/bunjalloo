#ifndef WizardCPU_h_seen
#define WizardCPU_h_seen

class Wizard;

#include "Computer.h"
class WizardCPU : public Computer
{

  public:
    WizardCPU(Wizard & theWizard);

    void aiCastCreature();
    void doAISpell();
  private:
    bool m_targetSquareFound;
    int m_tableIndex;
    Wizard & m_wizard;
    int m_targetCount;

    int getStrongestWizard(int attackerIndex);
    void resetPriorityTable();
    int createRangeTable(int target, int range);
    void getFurthestInrange();
    void createTableEnemies();

};
#endif
