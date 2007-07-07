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
    //! The height of the arena in 8x8 tiles.
    static const int HEIGHT;
    //! The size (width*height) of the arena
    static const int ARENA_SIZE;

    static const int SCREEN;
    static const int ALT_SCREEN;

    //! X position of top left corner in pixels
    static const int POSITION_X;
    //! Y position of top left corner in pixels
    static const int POSITION_Y;
    //! Width of arena in pixels
    static const int PIXEL_WIDTH;
    //! Height of arena in pixels
    static const int PIXEL_HEIGHT;

    /** Get the distance between 2 squares.
     * @param square1 the start index, equal to x + y * 16
     * @param square2 the start index.
     */
    static int distance(int square1, int square2);
    
    /*! @brief Get the arena.
     * @return singleton instance of an Arena.
     */
    static Arena & instance();

    /** Get the x and y coordinates that correspond to the given index.
     * @param index the index to convert to x, y coordinates.
     * @param x is filled with the x coord value.
     * @param y is filled with the y coord value.
     */
    static void getXY(int index, int & x, int & y);
    
    /** Convert index from a square to one of its surroundings.
     * @return index+1, index 0 means out of bounds.
     */
    static int applyPositionModifier(int square, int index);

    /** Get the attack preference value fot the given spell. */
    static int attackPref(int c);

    /*! @brief Clear the graphics for all squares. */
    void clear();

    /*! Reset (empty) the arena values. This should be called at the end of a game. */
    void reset();

    /*! @brief Counts down the timer square values and updates the anim frame value if needed. */
    void countdownAnim();
    
    /*! @brief Draw a decorative border around the screen
     * @param pal the palette to use
     * @param col1 colour 1 of the frame in RGB format
     * @param col2 colour 2 of the frame in RGB format
     * @param height the height of the border, default Y_LIMIT
     */
    void decorativeBorder(int pal, unsigned short col1,  unsigned short col2, int height=HEIGHT, int screen=SCREEN);

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

    /** The types of cursor graphics to display.*/
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

    /** Draws the cursor by setting the data in the sprite RAM.
     * @param type The type of cursor to draw.
     */
    void drawCursor(Cursor_t type=CURSOR_NORMAL_GFX);

    //! Moves the cursor up one square.
    void cursorUp();
    //! Moves the cursor down one square.
    void cursorDown();
    //! Moves the cursor left one square.
    void cursorLeft();
    //! Moves the cursor right one square.
    void cursorRight();

    //! do not move the cursor, just set it as is.
    void cursorSet();

    /** Enable or disable the cursor. Turns the sprite that represents the cursor on (or off).
     * @param enable Enable the cursor or not. True to enable, false to disable.
     */
    void enableCursor(bool enable=true);

    //! clear a square here
    void clearSquare(int x, int y);

    //! Clear the game border from the map data.
    void clearGameBorder();

    //! return the creature here
    int cursorContents() const;
    /** Fetch the contents of the arena under the current cursor position.
     * @param theCreature is filled with the creature here.
     * @param theOneUnderneath is filled with the details of the creature underneath.
     * @param theFlags is filled with the flags for the creature.
     */
    void cursorContents(int & theCreature, int & theOneUnderneath, int & theFlags) const;
    /** Fetch the contents of the arena under the current cursor position.
     * @param theCreature is filled with the creature here.
     * @param theOneUnderneath is filled with the details of the creature underneath.
     * @param theFlags is filled with the flags for the creature.
     * @param theFrame is filled with the frame for theCreature.
     */
    void cursorContents(int & theCreature, int & theOneUnderneath, 
        int & theFlags, int & theFrame) const;

    //! Reset the animation details in arena 1 and 2 based upon the info in arena 0
    void resetAnimFrames();

    /** Set the spell (creature) at the given position.
     * @param x the arena x coordinate.
     * @param y the arena y coordinate.
     * @param spellID the spell number.
     */
    void setSpellAt(int x, int y, int spellID);

    /** Fetch the contents of a given table at the given index.
     * @param i The table to use.
     * @param j the index into the table.
     */
    inline int at(int i, int j)
    {
      return m_arena[i][j];
    }

    /** Fetch the contents of table 0 at the current target square.
     * @return the contents of table 0 at the current target square.
     */
    inline int atTarget()
    {
      return m_arena[0][m_targetIndex];
    }

    /** Fetch the contents of table 4 at the current target square.
     * @return the contents of table 4 at the current target square.
     */
    inline int underTarget()
    {
      return m_arena[4][m_targetIndex];
    }

    /** get the current round number.
     * @return the current round number.
     */
    int roundNumber() const;

    /** Set the round counter to 0. */
    void resetRoundCount();

    //! set the target, wizard and start index information for the current player.
    void setCurrentPlayerIndex();

    //! get the x and y coordinates for the current player. (where is this used?)
    void currentPlayerXY(int & x, int &y);
    
    /** Redraws all creatures. Needs to be public to animate arena. */
    void drawCreatures();

    /*! @brief check if the currently selected spell can be cast to where the target square.
     * @param spellId the spell id to check the range of
     * @param printMessage if true then the message "out of range" is drawn if the check fails.
     * @return true if the spell is in range, false otherwise.
     */
    bool isSpellInRange(int spellId, bool printMessage) const;

    /*! @brief check if there is a tree next to where the current spell is being cast.
     * @param spellId the spell being cast.
     * @return true if there is a tree next to the current target, false otherwise.
     */
    bool isTreeAdjacent(int spellId) const;
    /** Check if a wall is adjacent to the casting wizard.
     * @param spellId The spell that is being cast.
     * @param wizardId The wizard who is casting the spell
     * @return true if there is a wall-like spell next to the casting wizard, false otherwise.
     */
    bool isWallAdjacent(int spellId, int wizardId) const;

    /** Check for Line of sight.
     * @param printMessage if true then the message "No line of sight" is drawn in this case.
     * @return true if the line of sight between the start index and the
     * target index is blocked. returns false if it is not 
     */
    bool isBlockedLOS(bool printMessage) const;

    //! Fetch the X and Y positions of the current target index. (where is this used?)
    void targetXY(int & x, int & y);

    /** Draws a frame of the spell cast animation.
     * @param x the x position of where to draw,
     * @param y the y position of where to draw.
     * @param frame the frame of the animation to draw.
     */
    void drawSpellcastFrame(int x, int y, int frame);
    /** Finish the creation of a creature when a creature spell succeeds.
     * Places the creature in the arena and sets the flags for the current player. 
     */
    void creatureSpellSucceeds();

    /** @return the id of the enemy at the given index
     * @param index the arena index to check.
     */
    int containsEnemy(int index);
    /** @return the owner id of the creature in the given square.
     * @param index the arena index to check.
     */
    int owner(int index) const;

    /** Get the index of the arena that holds the player given by id. 
     * Note that it is not the same as the m_wizardIndex version below!
     * @param id the number of the player to get.
     * @return the index in the arena that holds the player with id @a id
     */
    int wizardIndex(int id) const;

    /** Get the id of the wizard at the given index.
     * @param index the arena index to check.
     * @return the identifier of the wizard at the @a index
     */
    int wizardId(int index) const;
    
    /** Does the square contain an undead. Undead may be from the flags or
     * "natural" behaviour of a creature (vampire, zombie, etc).
     * @param index the index square to check.
     * @return true if the square contains an undead creature/thing.
     */
    bool isUndead(int index) const;
    
    /** has the cpu cast disbelieve on the given index.*/
    bool hasDisbelieveCast(int index) const;

    //! Set the "has moved" flag on the given index.
    void setHasMoved(int index);
    //! @return true if the given index contains a creature that has already moved.
    bool hasMoved(int index) const;

    /** is the indexed square an illusion.  */
    bool isIllusion(int index);
    /** Set the flag that indicates the thing in this square has had disbelieve cast on it. */
    void setDisbelieved(int index);
    /**
     * Shuffles the things about in the arena table for when disbelieve 
     * is cast successfully. Checks for old bugs option for when disbelieve 
     * is cast and a dead body is underneath. Casts on current targetIndex
     */
    void doDisbelieve();

    /** @return the the index of the starting square */
    inline int startIndex() const { return m_startIndex; }
    /** @return the the index of the wizard square */
    inline int wizardIndex() const { return m_wizardIndex; }
    /** @return the the index of the target square */
    inline int targetIndex() const { return m_targetIndex; }
    /** @param i the the index of the start square */
    inline void setStartIndex(int i) { m_startIndex = i; }
    /** @param i the the index of the wizard square */
    inline void setWizardIndex(int i) { m_wizardIndex = i; }
    /** @param i the the index of the target square */
    inline void setTargetIndex(int i) { m_targetIndex = i; }

    //! start the next round (what is it doing in arena?)
    void nextRound();

    /** highlight all the creatures belonging to the given player.
     * @param playerId the player whose creatures are highlighted.
     */
    void highlightCreatures(int playerId);
    /** Highlight the creatures that belong to the current target. */
    void highlightTargetCreations(); 

    /** Checks the frame - if it is 4 then the creature here is dead.
     * @return true if there is a dead creature in this square.
     * @param index the square to check.
     */
    bool isDead(int index) const;

    /** Checks the flag - if the blindness bit is set and the frame is not 4
     * then the creature here is blind.
     * @return true if there is a blind creature in this square.
     * @param index the square to check.
     */
    bool isBlind(int index) const;

    /** Checks the flag - if the blindness bit is set and the frame is 4
     * then the creature here is asleep.
     * @return true if there creature in this square is asleep.
     * @param index the square to check.
     */
    bool isAsleep(int index) const;

    /** Move selected creature from start index to target index.
     * @return true if the move caused a wizard to end its turn.
     */
    bool doSuccessfulMove(int selectedCreature);

    /** Helper for movement checks. */
    void ownerAt(int x, int y, int &surround_creature) const;

    /** helper function for movement. Removes the creature at targetindex*/
    void killCreature();

    //! Cause the current target square to turn into a corpse.
    void leaveCorpse();
    //! Remove an undead creature at the target square and clean up underneath it.
    void killUndead();
    //! Implement the turmoil random motion.
    void turmoil();
    //! Show a "pop" animation at the target square.
    void popAnimation();
    //! Show a "splat" animation at the target square.
    void splatAnimation();
    /** Does the last part of the magic missile type spells. Removes the
     * creature, cleans up underneath.
     */
    void magicMissileEnd();
    //! Perform the sleep or blind action on the target square.
    void sleepBlind();
    //! Perform justice on the current target square.
    void justice();

    //! Draws the silhoutte justice animation at the target square.
    inline void drawSilhouetteGfx(
        const unsigned short * gfx, 
        const unsigned short * map,
        unsigned short col)
    {
      drawSilhouetteGfx(m_targetIndex, gfx, map, col, 11, false);
    }; 
    /** Destroys all the creatures belonging to the given wizard.
     * @param playerid the unlucky player.
     */
    void destroyAllCreatures(int playerid);

    //! raise the creature at target index and set to current player
    void raiseDead();
    //! subvert the creature at target index and set to current player
    void subvert();

    /*! Mutate the creature at target index, or all of the wizards creatures if mutating a wizard.  
     * @return true if the mutation succeeds, false otherwise.
     */
    bool mutate();

    //! Draw the wizard death animation using the given image index.
    void wizardDeath(int image);

  private:
    nds::Background * m_bg;
    nds::Background * m_alternateBg;
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
           | | | | +--------- is asleep/blind
           | | | +----------- is illusionary
           | | +------------- has had disbelieve cast on it
           | +--------------- undead
           +----------------- "has been spread" for gooey blob and fire, has moved for everything else
                               is asleep when underneath a blob spread
    */
    unsigned char m_arena[6][160];  

    // helper fns for the index handling
    static void getXY2(int index, int & x, int & y);
    
    Arena();
    ~Arena();

    /*! @brief Initialise the arena background map values.  */
    void initialiseMap();

    void gameBorder();
    void drawCreature(int x, int y, int creature, int frame);

    // set the current index for all targets
    void setCurrentIndex(int i);

    /** called at the end of the moves round
     * resets the movement flags so that the creatures can move next time
     */
    void unsetMovedFlags();
    void spreadFireBlob();
    void destroyCastles();
    void randomNewSpell();
    /** When a wizard meditates, set the hasMoved flag so the wizard stands
     * still
     */
    void freezeMeditatingWizards();

    /** Update sleeping/blind creatures. There is a random chance that the
     * effect wears off.  If the effect does not wear off and the creature's
     * asleep, then set the hasMoved flag.
     */
    void updateSleepers();

    void liberateFromBlob(int index);

    void displayCursorContents();
    void drawSilhouetteGfx(
        int arenaIndex, 
        const unsigned short * gfx, 
        const unsigned short * map,
        unsigned short col, 
        int palette,
        bool negative); 
    void drawPopFrame(int x, int y, int frame);
    void uncoverSquare(int start, int target);
    void doSpread(int start, int target);
    bool blindBitSet(int index) const;
};

#endif
