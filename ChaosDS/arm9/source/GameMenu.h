#ifndef GameMenu_h_seen
#define GameMenu_h_seen

#include "ScreenI.h"

class GameMenu: public ScreenI
{
  public:
    //! Continue the game, corresponds to option 3 on the menu.
    static void continueGame();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_hilightItem;

    void deselectItem(int item);
    inline void deselectItem() { deselectItem(m_hilightItem); };
    void selectItem();
    void up(); 
    void down();
    void a();
};
#endif
