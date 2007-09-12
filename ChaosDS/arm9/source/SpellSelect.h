/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
