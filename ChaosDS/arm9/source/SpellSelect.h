#ifndef SpellSelect_h_seen
#define SpellSelect_h_seen

#include "ScreenI.h"

class SpellSelect: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
  private:
    int m_hilightItem;
    int m_topIndex;

    void initPalettes();
    void listSpells();
#if 0

    void deselectItem(int item);
    inline void deselectItem() { deselectItem(m_hilightItem); };
    void selectItem();
    void up(); 
    void down();
    void a();
#endif
};
#endif
