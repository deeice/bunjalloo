#ifndef Text16_h_seen
#define Text16_h_seen
#include "Background.h"
#include "Palette.h"

class Text16
{

  public:
    //! Index of the first character.
    static const int FIRST_CHAR_INDEX;
    static const int MESSAGE_X;
    static const int MESSAGE_Y;
    //! Singleton instance.
    static Text16 & instance();
    //! Clear all text.
    void clear();
    /*! @brief set the given palette to the given RGB colour value for text.
     * @param palette the index of the palette to use (0-15)
     * @param colour the colour to use in RGB format
     */
    void setColour(int palette, int colour);
    
    /*! @brief get the colour value for the text from the given palette.
     * @param palette the index of the palette to get (0-15)
     * @return colour used in the palette, in RGB format.
     */
    unsigned short colour(int palette) const;

    /*! @brief print the 16 point text. 
     * Each letter is 2 tiles big. the first tile goes on row y, the second on row y+1
     * @param text the characters to print to the screen
     * @param x the x position
     * @param y the y position
     * @param pal the palette to use
     */
    void print(const char* text, int x, int y , int pal);

    /*! @brief Place a single character from the alphabet at position x, y. 
     * This is used to draw, eg. the ^/v arrow thing
     * @param ch the character to draw,
     * @param x the x tile position of where to draw
     * @param y the y tile position of where to draw
     * @param pal the palette index to use
     * @param extraFlags any extra flags to pass to the map (flip flags for example)
     */
    void putChar(int ch, int x, int y, int pal, unsigned int extraFlags);

    /*! @brief clear the in game message */
    void clearMessage();

    /*! @brief Show an in-game message.
     * @param message The message to display
     */ 
    void displayMessage(const char * message, 
        unsigned short color=nds::Color(0,30,30));

    /*! @brief convert an integer to a character string using decimal format.
     * @param n the integer value
     * @param str the array to store the return value in
     */
    static void int2a(int n, char str[]);

    //! The number of letter glyphs available.
    static const int MAX_LETTERS;

  private:
    int m_screen;
    nds::Background * m_bg;
    int m_mapOffset;
    Text16();
    ~Text16();
};
#endif
