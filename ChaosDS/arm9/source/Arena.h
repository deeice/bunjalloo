#ifndef Arena_h_seen
#define Arena_h_seen

#include "Background.h"

class Arena
{
  public:
    //! number that represents a wizard-type creature
    static const int WIZARD_INDEX;
    static const int HEIGHT;
    /*! @brief Get the arena.
     * @returns singleton instance of an Arena.
     */
    static Arena & instance();
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
     * @returns the number of active players
     */
    int players() const;
    /*! @brief set the number of players
     * @param p the number of players (2-8)
     */
    void players(int p);

    /*! @brief fetch the index of the current player.
     * @returns the index of the current player
     */
    int currentPlayer() const;
    /*! @brief Set the current player index.
     * @param cp the index of the current player
     */
    void currentPlayer(int cp);

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

    //! set the border colour to match the given player
    void setBorderColour(unsigned char playerid);

    //! clear a square here
    void clearSquare(int x, int y);
  private:
    nds::Background * m_bg;
    int m_playerCount;
    int m_currentPlayer;
    
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
    static void getXY(int index, int & x, int & y);
    static void getXY2(int index, int & x, int & y);
    
    Arena();
    ~Arena();
    void gameBorder();
    void drawCreatures();
};

#endif
