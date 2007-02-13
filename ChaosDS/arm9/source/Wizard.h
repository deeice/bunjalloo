#ifndef Wizard_h_seen
#define Wizard_h_seen

struct SpellData;
class Wizard
{

  public:
  
    static void resetPlayers();
    static void createDefaultWizards();
    static Wizard * getPlayers();
    static Wizard & getCurrentPlayer();
    static void initialisePlayers();
    static int getNextHuman(int startIndex=0);

    void draw8(int x, int y, int frame);
    void updateColour();
    //! print the wizard name at x, y, using the given palette
    //! if no palette is given, then use the wizard id
    void nameAt(int x, int y, int pal=-1);
    void printLevelAt(int x, int y);

    int level() const;
    void level(int);

    int colour() const;
    void colour(int );

    int image() const;
    void image(int);

    bool isCpu() const;
    bool isDead() const;

    //! removes used spells from spell list and resorts
    //! human wizards only
    void removeNullSpells();

    const char * const name();

    const SpellData * spell(int) const;
    int spellCount() const;

    enum Player_t {
      PLYR_HUMAN=0,
      PLYR_CPU
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
