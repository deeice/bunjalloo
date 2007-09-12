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
#ifndef Wizard_h_seen
#define Wizard_h_seen

struct SpellData;
class WizardCPU;
#include "Computer.h"

class Wizard : public Computer
{

  public:
    //! The graphic map locations for each graphic segment.
    static const unsigned short DEFAULT_MAP[];
    //! The index into the palette for the wizard colour.
    static const int WIZARD_COLOUR;
  
    /*! @brief set the player instances to their initial state */
    static void resetPlayers();
    /*! @brief create some default wizards for all players */
    static void createDefaultWizards();
    /*! @brief get the array of player instances */
    static Wizard * players();

    /*! @brief get a player */
    static Wizard & player(int index);

    /*! @brief get the current player wizard.
     * @return a reference to the current player
     */
    static Wizard & currentPlayer();
    /*! @brief initialise the player wizards to the default values. */
    static void initialisePlayers();
    /*! @brief get the next wizard that is not CPU controlled. */
    static int nextHuman(int startIndex=0);
    /** Get the current dead wizard count. */
    static int deadWizards();
    //! Set the number of dead wizards back to 0.
    static void resetDeadWizards();
    //! Increment the dead wizard count by 1.
    static void incrDeadWizards();

    
    /** Draw the justice graphic frame for the wizard. Draws a multicoloured silhoutte.
     * @param frame the frame number to use.
     */
    void drawJusticeGfx(int frame) const;

    /*! @brief draw a wizard glyph at the given tile position.
     * @param x the x tile position.
     * @param y the y tile position.
     * @param frame the frame to draw.
     */
    void draw8(int x, int y, int frame);

    /*! @brief draw a wizard glyph at the given arena position.
     * @param x the x position.
     * @param y the y position.
     * @param frame the frame to draw.
     */
    void draw(int x, int y, int frame);

    /*! @brief update the colour based on id
     */
    void updateColour();

    /*! @brief Print the wizard name at x, y, using the given palette.
     * if no palette is given, then use the wizard id
     * @param x the tile x position
     * @param y the tile y position
     * @param pal the palette index to use
     */
    void printNameAt(int x, int y, int pal=-1) const;
    //! print the CPU level or human at the given tile coords.
    void printLevelAt(int x, int y) const;

    //! @return true if has magic wings.
    inline bool hasMagicWings() const
    {
      return m_modifierFlag & 0x20;
    }
    //! @return true if has shadow form.
    inline bool hasShadowForm() const
    {
      return m_modifierFlag & 0x8;
    }
    //! Set if the wizard has shadow form or not.
    inline void setHasShadowForm(bool has)
    {
      if (has) {
        m_modifierFlag |= 0x8;
      } else {
        m_modifierFlag &= ~0x8;
      }
    }
    //!@return the attack modifier gained from magic sword/knife etc.
    inline int attackModifier() const
    {
      return m_modifierFlag & 7;
    }
    //!@return the defence modifier gained from magic shields.
    inline int defenceModifier() const
    {
      return (m_modifierFlag & 0xc0)>>6;
    }

    //!@return true if the wizard has a magic shield.
    inline bool hasMagicShield() const
    {
      return m_modifierFlag & 0x40;
    }
    //!@return true if the wizard has magic armour.
    inline bool hasMagicArmour() const
    {
      return m_modifierFlag & 0x80;
    }
    //!@return true if the wizard has a magic sword.
    inline bool hasMagicSword() const
    {
      return m_modifierFlag & 0x04;
    }
    //!@return true if the wizard has a magic knife.
    inline bool hasMagicKnife() const
    {
      return m_modifierFlag & 0x02;
    }
    //!@return the basic combat stat.
    inline int combat() const { return m_combat; }
    //!@return the basic defence stat.
    inline int defence() const { return m_defence; }

    //! Set the flag that enables magic shield.
    void setHasMagicShield();
    //! Set the flag that enables magic armour.
    void setHasMagicArmour();
    //! Set the flag that enables magic sword.
    void setHasMagicSword();
    //! Set the flag that enables magic knife.
    void setHasMagicKnife();
    //! Set the flag that enables magic bow.
    void setHasMagicBow();
    //! Set the flag that enables magic wings.
    void setHasMagicWings();

    //! @return the current level. 0 is human.
    int level() const;
    //! set the current level. 0 is human.
    void setLevel(int);

    //! @return the "real" colour of this wizard in RGB format.
    unsigned short colourRGB() const;
    
    //! @return the colour index of this wizard.
    int colour() const;

    //! set the colour index of this wizard.
    void setColour(int );

    //! @return the image index of this wizard.
    int image() const;
    //! set the image index of this wizard.
    void setImage(int);

    //! @return true if this wizard is CPU controlled.
    bool isCpu() const;
    //! @return true if this wizard is dead.
    bool isDead() const;

    //! removes used spells from spell list and resorts
    //! human wizards only
    void removeNullSpells();

    //! @return the name of this wizard.
    const char * const name() const;

    /** Set the name of the wizard. The @a name is copied into m_name.
     * @param name the new name of the wizard (12 chars max).
     */
    void setName(const char * name);

    //! @return pointer to the spell at the given index
    //! @param index the index of the spell, 0-19
    const SpellData * spell(int index) const;

    //! @return the spell id at the given index
    //! @param index the index of the spell, 0-19
    const int spellId(int index) const;

    //! @return the total number of spells available.
    int spellCount() const;
    /** Order the spells for the CPU player. */
    void orderSpells();

    /** Add a spell to the list. Places it in an empty slot.*/
    void addSpell(int spellId);

    //! @return the ability flag.
    int ability() const;

    //! helper function for the examine screen
    void displayData() const;

    /*! @brief Set the spell that has been chosen from the list
     * @param index the spell index in the list
     */
    void setSelectedSpell(int index);

    //!@return the currently selected spell identifier. For example SPELL_GOOEY_BLOB.
    int selectedSpellId() const;

    //! set no spell...
    void removeSelectedSpell();

    //! Set up the cast stage for a human wizard.
    void setupHumanPlayerCast();

    /*! @brief Set if the spell to be cast is an illusion.
     * @param isIllusion true for illusion spells.
     */
    void setIllusionCast(bool isIllusion);
    //!@return true if the wizard has chosen to cast an illusion.
    inline bool illusionCast()const
    {
      return m_illusionCast;
    }

    //!Make this player wait until he presses a key.
    void waitForKeypress();
    //! Update the bravery of this player based on round number and creature count.
    void updateBravery();

    /** Get the number of non-mounted creatures that belong to this wizard.
     * @param includeBlobs true if we should include gooey blobs.
     * @return the number of creatures
     */
    int creatureCount(bool includeBlobs) const;

    //! Set up the generic part needed for casting range 0 spells (magic knife, etc)
    void setupRange0Spell();

    //!@return the cast amount.
    inline int castAmount() const
    {
      return m_castAmount;
    }
    //!Set teh cast amount.
    inline void setCastAmount(int ca) {
      m_castAmount = ca;
    }

    //! @return true if the current spell can be cast tot he chosen location.
    bool isCastAllowed() const;

    //! Add a new random spell and print name.
    void newRandomSpell();

    //! Player types.
    enum Player_t {
      PLYR_HUMAN=0, //!< indicates that is human
      PLYR_CPU      //!< indicates that is CPU controlled
    };

    //! Print the player name and chosen spell.
    void printNameSpell() const;
    //!@return the timid value/current creatures.
    inline int timid() const { return m_timid; }

    //!Priority offset is used for fiddling AI stuff.
    inline int priorityOffset() const { return m_priorityOffset; }
    //!Priority offset is used for fiddling AI stuff.
    inline void setPriorityOffset(int offset) { m_priorityOffset = offset; }

    //!@return this wizards id/number.
    inline int id() const { return m_id; }

    // Computer routines
    virtual void aiCast(int);
    virtual void doAiSpell();
    virtual void doAiMovement();

    bool hasTargetSquare() const;
    void setHasTargetSquare(bool);

    //!@return the pointer to the computer implementation.
    inline Computer * computer() const
    {
      return m_computer;
    }

    //!@return the ranged combat value. Only valid if has magic bow.
    inline int rangedCombat() const
    {
      return m_rangedCombat;
    }
    //!@return the ranged combat range. Only valid if has magic bow.
    inline int range() const
    {
      return m_range;
    }

    //!Kill this wizard, performing the animation and removing creatures.
    void kill();

    //!@return the magic resistance.
    inline int magicResistance() const
    {
      return m_magicResistance;
    }

    //!Get the spell id of the last spell that the player cast.
    inline int lastSpellCast() const
    {
      return m_lastSpellCast;
    }

  private:
    char m_name[12];
    unsigned char m_combat;
    unsigned char m_rangedCombat;
    unsigned char m_range;
    unsigned char m_defence;
    unsigned char m_movementAllowance;
    unsigned char m_manoeuvreRating;
    unsigned char m_magicResistance;
    unsigned char m_spellCount;
    //! 21 spells. 2 byte pairs, one for the priority, the other for the spell id
    unsigned char m_spells[42];  
    unsigned char m_ability;
    unsigned char m_image;
    unsigned short m_colour;
    unsigned char m_playerType;
    /**
     *  The meaning of bits in modifier flag are as follows.
     *  bit 0    < not used ? >
     *  bit 1    magic knife
     *  bit 2    magic sword
     *  bit 3    shadow form
     *  bit 4    is dead
     *  bit 5    magic wings
     *  bit 6    magic shield
     *  bit 7    magic armour
     */
    unsigned char m_modifierFlag;
    unsigned char m_illusionCast;
    unsigned char m_selectedSpell;
    unsigned char m_lastSpellCast;
    unsigned char m_timid; //!< Either is timid or has creatures, not sure.
    //! player id - used for colouring in
    unsigned char m_id;
    unsigned char m_castAmount; //! temp_cast_amount in original C code.
    Computer * m_computer;
    unsigned char m_priorityOffset;

    Wizard();
    ~Wizard();
    void reset();
    
    static Wizard s_players[8];
    static int s_deadWizards;
	
};
#endif
