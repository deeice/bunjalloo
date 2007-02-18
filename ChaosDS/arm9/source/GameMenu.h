#ifndef GameMenu_h_seen
#define GameMenu_h_seen

#include "ScreenI.h"

class GameMenu: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
  private:
    int m_hilightItem;

    void deselectItem(int item);
    inline void deselectItem() { deselectItem(m_hilightItem); };
    void selectItem();
    void up(); 
    void down();
    void a();
    void continueGame();
};
#endif
