#ifndef ButtonGroup_h_seen
#define ButtonGroup_h_seen

#include <vector>
#include "ButtonListener.h"

class ButtonGroup: public ButtonListener
{
  public:
    
    void add(ButtonI * button);
    void remove(ButtonI * button);

    virtual void pressed(ButtonI * button);

  private:
    std::vector<ButtonI*> m_group;
};

#endif
