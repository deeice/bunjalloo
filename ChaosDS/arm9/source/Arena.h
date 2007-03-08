#ifndef Arena_h_seen
#define Arena_h_seen

namespace nds {
  class Background;
  class Sprite;
};
class Line;
class Arena
{
  friend class Line;
  public:
    //! number that represents a wizard-type creature
    static const int WIZARD_INDEX;
    static const int HEIGHT;

    /** Get the distance between 2 squares.
     * @param square1 the start index, equal to x + y * 16
     * @param square1 the start index.
     */
    static int distance(int square1, int square2);
    
    /*! @brief Get the arena.
     * @return singleton instance of an Arena.
     */
    static Arena & instance();

    static void getXY(int index, int & x, int & y);

    /*! @brief Reset the arena map values.  */
    void reset();

    /*! @brief Clear the graphics for all squares. */
    void clear();

    /*! @brief Counts down the timer square values and updates the anim frame value if needed. */
    void countdownAnim();
    
    /*! @brief Draw a decorative border around the screen
     * @param pal the palette to use
     * @param col1 colour 1 of the frame in RGB format
     * @param col2 colour 2 of the frame in RGB format
     * @param height the height of the border, default Y_LIMIT
     */
    void decorativeBorder(int pal, unsigned short col1,  unsigned short col2, int height=HEIGHT);

    /*! @brief Set the palette used at the given location
     * @param x the x position 0-29
     * @param y the y position 0-19
     * @param pal the palette index
     */
    void setPalette8(int x, int y, int pal);

    /*! @brief fetch the number of active players 
     * @return the number of active players
     */
    int players() const;
    /*! @brief set the number of players
     * @param p the number of players (2-8)
     */
    void setPlayers(int p);

    /*! @brief fetch the index of the current player.
     * @return the index of the current player
     */
    int currentPlayer() const;
    /*! @brief Set the current player index.
     * @param cp the index of the current player
     */
    void setCurrentPlayer(int cp);

    /*! @brief set a square to contain the given wizard id
     * @param square the index of the arena square to set
     * @param id the index of the wizard (0-7)
     */
    void setWizardSquare(int square, int id);
    /*! @brief draw an 16x16 graphics glyph at the given position
     * @param gfx pointer to the graphics data
     * @param map pointer to the map data
     * @param x position of the square to start at (0-29)
     * @param y position of the square to start at (0-19)
     * @param frame the index of the frame to use (0-3, usually)
     */
    void drawGfx8(
        const unsigned short * gfx,
        const unsigned short * map,
        int x, int y, int frame
        );

    /*! @brief display the arena in all its glory...
     */
    void display();

    typedef enum {
      CURSOR_SPELL_GFX,
      CURSOR_ENGAGED_GFX,
      CURSOR_FIRE_GFX,
      CURSOR_FLY_GFX,
      CURSOR_GROUND_GFX,
      CURSOR_NORMAL_GFX,
      CURSOR_NOT_DRAWN
    } Cursor_t;
    //! set the border colour to match the given player
    void setBorderColour(unsigned char playerid);

    /*! @brief set the cursor position and possibly draw it.
     * @param x the cursor tile position
     * @param y the cursor tile position
     * @param type cursor type to draw the cursor
     */
    void initialiseCursor(int x, int y, Cursor_t type=CURSOR_NORMAL_GFX);

    /*! @brief set the cursor position without updating drawing style.
     * @param x the cursor tile position
     * @param y the cursor tile position
     */
    void setCursor(int x, int y);

    void drawCursor(Cursor_t type=CURSOR_NORMAL_GFX);

    void cursorUp();
    void cursorDown();
    void cursorLeft();
    void cursorRight();
    void enableCursor(bool enable=true);

    //! clear a square here
    void clearSquare(int x, int y);

    //! return the creature here
    int cursorContents() const;
    void cursorContents(int & theCreature, int & theOneUnderneath, int & theFlags) const;
    void cursorContents(int & theCreature, int & theOneUnderneath, 
        int & theFlags, int & theFrame) const;

    //! Reset the animation details in arena 1 and 2 based upon the info in arena 0
    void resetAnimFrames();

    void setSpellAt(int x, int y, int spellID);

    inline int at(int i, int j)
    {
      return m_arena[i][j];
    }

    //! get the current round number
    int roundNumber() const;

    // set the target information for the current player
    void setCurrentPlayerIndex();

    void currentPlayerXY(int & x, int &y);
    
    //! Needs to be public to animate arena
    void drawCreatures();

    /*! @brief check if the currently selected spell can be cast to where the target square.
     * @param spellId the spell id to check the range of
     * @return true if the spell is in range, false otherwise.
     */
    bool isSpellInRange(int spellId) const;

    /*! @brief check if there is a tree next to where the current spell is being cast.
     * @param spellId the spell being cast.
     * @return true if there is a tree next to the current target, false otherwise.
     */
    bool isTreeAdjacent(int spellId) const;
    bool isWallAdjacent(int spellId, int wizardId) const;
    bool isBlockedLOS() const;

    void targetXY(int & x, int & y);

    void drawSpellcastFrame(int x, int y, int frame);
    void creatureSpellSucceeds();

    bool containsEnemy(int index);
    int owner(int index) const;
    //! fixme; nothing to do with the m_wizardIndex below!
    int wizardIndex(int id) const;
    int wizardId(int index) const;
    int attackPref(int c) const;
    bool isUndead(int index) const;


    inline int startIndex() const { return m_startIndex; }
    inline int wizardIndex() const { return m_wizardIndex; }
    inline int targetIndex() const { return m_targetIndex; }
    inline void setStartIndex(int i) { m_startIndex = i; }
    inline void setWizardIndex(int i) { m_wizardIndex = i; }
    inline void setTargetIndex(int i) { m_targetIndex = i; }

    // start the next round (what is it doing in arena?)
    void nextRound();

    void highlightCreatures(int playerId);
    void highlightTargetCreations(); 
    
  private:
    nds::Background * m_bg;
    nds::Sprite * m_cursor;
    int m_playerCount;
    int m_currentPlayer;
    typedef struct {
      short x;
      short y;
    } CursorPosition_t;
    CursorPosition_t m_cursorPosition;
    int m_roundNumber;
    int m_wizardIndex;  // index into arena of current player
    int m_startIndex;   // index into arena for start square of current spell
    int m_targetIndex;  // index into arena for target square of current spell
    short m_highlightCreations;  // hack for showing highlighted creations
    short m_highlightCreationsCounter;  // hack for showing highlighted creations
    
    
    // the 6 arena tables... 960 bytes
    /*
       arena 0 - the creature index 
       arena 1 - the timer square - when this reaches 0, update the frame in arena 2
       arena 2 - the anim frame, 4 is dead.
       arena 3 - modifier flag - bits are important, see below
       arena 4 - the creature index for creatures under the thing in arena 0
       arena 5 - modifier flag for things in arena 4

       arena 3 bit flag:
       MSB 7 6 5 4 3 2 1 0 LSB
           | | | | | | | +-\
           | | | | | | +----- the owner (or player id for wizards)
           | | | | | +-----/ 
           | | | | +--------- is asleep
           | | | +----------- is illusionary
           | | +------------- has had disbelieve cast on it
           | +--------------- undead
           +----------------- "has been spread" for gooey blob and fire
    */
    unsigned char m_arena[6][160];  

    // helper fns for the index handling
    static void getXY2(int index, int & x, int & y);
    
    Arena();
    ~Arena();
    void gameBorder();
    void drawCreature(int x, int y, int creature, int frame);

    // set the current index for all targets
    void setCurrentIndex(int i);
    // from a square to one of its surroundings
    // returns index+1, if index is 0 it is out of bounds
    static int applyPositionModifier(int square, int index);

    /** called at the end of the moves round
     * resets the movement flags so that the creatures can move next time
     */
    void unsetMovedFlags();
    void spreadFireBlob();
    void destroyCastles();
    void randomNewSpell();

    void displayCursorContents();
    void drawSilhouetteGfx(
        int arenaIndex, 
        const unsigned short * gfx, 
        const unsigned short * map,
        unsigned short col, 
        int palette,
        bool negative); 
};

#endif
