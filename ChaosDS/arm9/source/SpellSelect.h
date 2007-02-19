#ifndef SpellSelect_h_seen
#define SpellSelect_h_seen

#include "ScreenI.h"

class SpellSelect: public ScreenI
{
  public:
    SpellSelect();
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
  private:
    int m_hilightItem;
    int m_topIndex;

    void initPalettes();
    void listSpells();

    void deselectSpell(int item);
    inline void deselectSpell() { deselectSpell(m_hilightItem); };
    void selectSpell();

    void up(); 
    void down();
    void left(); 
    void right();
    void r();
    void a();
  protected:
    void b();
};
#endif
