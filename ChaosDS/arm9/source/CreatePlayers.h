#ifndef CreatePlayers_h_seen
#define CreatePlayers_h_seen

#include "ScreenI.h"
class CreatePlayers: public ScreenI
{
  public:
    /** @param start flag to say if the screen is the first, or if it should
     * only redraw already existing wizard details. If true, then it will
     * create a new set of wizards.
     */
    CreatePlayers(bool start=true);

    virtual void show();
    virtual void animate();
    virtual void handleKeys();
    
  private:
    int m_hilightItem;
    int m_hilightWizard;
    bool m_start;

    void selectItem(int);
    void deselectItem();
    void updatePlayers(void);

    void up();
    void down();
    void left();
    void right();
    void a();
    void l();
    void r();
    void start();
    
};
#endif
