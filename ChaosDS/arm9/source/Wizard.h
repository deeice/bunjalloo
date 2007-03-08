#ifndef Wizard_h_seen
#define Wizard_h_seen

struct SpellData;
class WizardCPU;
#include "Computer.h"

class Wizard : public Computer
{

  public:
    static const unsigned short DEFAULT_MAP[];
  
    /*! @brief set the player instances to their initial state */
    static void resetPlayers();
    /*! @brief create some default wizards for all players */
    static void createDefaultWizards();
    /*! @brief get the array of player instances */
    static Wizard * players();

    /*! @brief get a player */
    static Wizard & player(int index);

    /*! @breif get the current player wizard.
     * @return a reference to the current player
     */
    static Wizard & currentPlayer();
    /*! @brief initialise the player wizards to the default values. */
    static void initialisePlayers();
    /*! @brief get the next wizard that is not CPU controlled. */
    static int nextHuman(int startIndex=0);

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

    inline bool hasMagicWings() const
    {
      return m_modifierFlag & 0x20;
    }
    inline bool hasShadowForm() const
    {
      return m_modifierFlag & 0x8;
    }

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

    //! @return pointer to the spell at the given index
    //! @param index the index of the spell, 0-19
    const SpellData * spell(int index) const;

    //! @return the spell id at the given index
    //! @param index the index of the spell, 0-19
    const int spellId(int index) const;

    //! @return the total number of spells available.
    int spellCount() const;
    inline unsigned char * spellArray()
    {
      return m_spells;
    }

    //! @return the ability flag.
    int ability() const;

    //! helper function for the examine screen
    void displayData() const;

    /*! @brief Set the spell that has been chosen from the list
     * @param index the spell index in the list
     */
    void setSelectedSpell(int index);
    int selectedSpellId() const;
    //! set no spell...
    void removeSelectedSpell();

    void setupHumanPlayerCast();
    /*! @brief Set if the spell to be cast is an illusion.
     * @param isIllusion true for illusion spells.
     */
    void setIllusionCast(bool isIllusion);
    inline bool illusionCast()const
    {
      return m_illusionCast;
    }

    void waitForKeypress();
    void updateCreatureCount();


    inline int castAmount() const
    {
      return m_castAmount;
    }
    inline void setCastAmount(int ca) {
      m_castAmount = ca;
    }

    bool isCastAllowed() const;

    enum Player_t {
      PLYR_HUMAN=0, //!< indicates that is human
      PLYR_CPU      //!< indicates that is CPU controlled
    };

    void printNameSpell() const;
    inline int timid() const { return m_timid; }

    inline int priorityOffset() const { return 0; }

    inline int id() const { return m_id; }

    // Computer routines
    virtual void aiCastCreature();
    virtual void doAiSpell();
    virtual void doAiMovement();


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
    unsigned char m_modifierFlag;
    unsigned char m_illusionCast;
    unsigned char m_selectedSpell;
    unsigned char m_timid; // ??
    //! player id - used for colouring in
    unsigned char m_id;
    unsigned char m_castAmount; // temp_cast_amount in original code.
    Computer * m_computer;

    Wizard();
    ~Wizard();
    void reset();
    
    static Wizard s_players[8];
	
};
#endif
