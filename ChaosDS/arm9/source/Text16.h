#ifndef Text16_h_seen
#define Text16_h_seen
#include "Background.h"

class Text16
{

  public:
    static const int FIRST_CHAR_INDEX;
    static Text16 & instance();
    void clear();
    void setColour(int palette, int colour);
    unsigned short getColour(int palette) const;
    /*! @brief print the 16 point text. 
     * Each letter is 2 tiles big. the first tile goes on row y, the second on row y+1
     * @param text the characters to print to the screen
     * @param x the x position
     * @param y the y position
     * @param pal the palette to use
     */
    void print(const char* text, int x, int y , int pal);
    void putChar(int ch, int x, int y, int pal, unsigned int extraFlags);
    static void int2a(int n, char str[]);

    static const int MAX_LETTERS;

  private:
    int m_screen;
    nds::Background * m_bg;
    int m_mapOffset;
    Text16();
};
#endif
