#ifndef GameMenu_h_seen
#define GameMenu_h_seen

#include "TouchScreen.h"

class GameMenu: public TouchScreen
{
  public:
    GameMenu();
    //! Continue the game, corresponds to option 3 on the menu.
    static void continueGame();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_hilightItem;

    static void continueCb(void * arg);
    static void examineCb(void * arg);
    static void selectSpellCb(void * arg);
    void deselectItem(int item);
    inline void deselectItem() { deselectItem(m_hilightItem); };
    void selectItem();
    void up(); 
    void down();
    void a();
};
#endif
