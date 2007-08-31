#ifndef ButtonGroup_h_seen
#define ButtonGroup_h_seen

#include <vector>
#include "ButtonListener.h"

/** Class to handle single selection across a group of buttons. */
class ButtonGroup: public ButtonListener
{
  public:
    
    /** Add a ButtonI to the group.
     * @param button the button to add.
     */
    void add(ButtonI * button);
    /** Remove a ButtonI from the group.
     * @param button the button to remove.
     */
    void remove(ButtonI * button);

    /** Called by a ButtonI when it is touched. Handles the exclusiveness of
     * presses amongst the button peers.
     * @param button the button that has been pressed.
     */
    virtual void pressed(ButtonI * button);

  private:
    std::vector<ButtonI*> m_group;
};

#endif
