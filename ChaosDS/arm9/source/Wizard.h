#ifndef Wizard_h_seen
#define Wizard_h_seen

struct SpellData;
class Wizard
{

  public:
  
    /*! @brief set the player instances to their initial state */
    static void resetPlayers();
    /*! @brief create some default wizards for all players */
    static void createDefaultWizards();
    /*! @brief get the array of player instances */
    static Wizard * getPlayers();
    /*! @breif get the current player wizard.
     * @returns a reference to the current player
     */
    static Wizard & getCurrentPlayer();
    /*! @brief initialise the player wizards to the default values. */
    static void initialisePlayers();
    /*! @brief get the next wizard that is not CPU controlled. */
    static int getNextHuman(int startIndex=0);

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

    //! @returns the colour value for this wizard
    unsigned short getColour();

    /*! @brief Print the wizard name at x, y, using the given palette.
     * if no palette is given, then use the wizard id
     * @param x the tile x position
     * @param y the tile y position
     * @param pal the palette index to use
     */
    void nameAt(int x, int y, int pal=-1);
    //! print the CPU level or human at the given tile coords.
    void printLevelAt(int x, int y);

    //! @returns the current level. 0 is human.
    int level() const;
    //! set the current level. 0 is human.
    void level(int);

    //! @returns the colour index of this wizard.
    int colour() const;
    //! set the colour index of this wizard.
    void colour(int );

    //! @returns the image index of this wizard.
    int image() const;
    //! set the image index of this wizard.
    void image(int);

    //! @returns true if this wizard is CPU controlled.
    bool isCpu() const;
    //! @returns true if this wizard is dead.
    bool isDead() const;

    //! removes used spells from spell list and resorts
    //! human wizards only
    void removeNullSpells();

    //! @returns the name of this wizard.
    const char * const name();

    //! @returns pointer to the spell at the given index
    //! @param index the index of the spell, 0-19
    const SpellData * getSpell(int index) const;
    //! @returns the total number of spells available.
    int spellCount() const;

    //! @returns the ability flag.
    int getAbility() const;

    enum Player_t {
      PLYR_HUMAN=0, //!< indicates that is human
      PLYR_CPU      //!< indicates that is CPU controlled
    };
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

    Wizard();
    void reset();
    
    static Wizard s_players[8];
	
};
#endif
