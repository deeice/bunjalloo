#ifndef SpellSelect_h_seen
#define SpellSelect_h_seen

#include "TouchScreen.h"

class ExamineSquare;

class SpellSelect: public TouchScreen
{
  public:
    /** Spell selection starts.
     * @param examine if true then the spell selection is really spell examine
     */
    SpellSelect(bool examine);
    ~SpellSelect();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_hilightItem;
    int m_topIndex;
    bool m_examine;
    ExamineSquare * m_examineScreen;

    static void scrollDownCb(void * arg);
    static void scrollUpCb(void * arg);
    static void spellSelectCb(void * arg);
    static void returnCb(void * arg);

    void initPalettes();
    void listSpells();

    void deselectSpell(int item);
    inline void deselectSpell() { deselectSpell(m_hilightItem); };
    void selectSpell();

    void scrollUp();
    void scrollDown();
    void up(); 
    void down();
    void left(); 
    void right();
    void r();
    void a();

    void examineSpell();
    void chooseSpell();
  protected:
    //! Cancel spell selection.
    void b();
};
#endif
