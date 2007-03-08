#ifndef CreatePlayers_h_seen
#define CreatePlayers_h_seen

#include "ScreenI.h"
class CreatePlayers: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
    
  private:
    int m_hilightItem;
    int m_hilightWizard;

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
