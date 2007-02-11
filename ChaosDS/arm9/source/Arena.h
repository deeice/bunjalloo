#ifndef Arena_h_seen
#define Arena_h_seen

#include "Background.h"

class Arena
{
  public:
    static Arena & instance();
    void reset();
    void clear();

    void countdownAnim();
    void decorativeBorder(int pal, unsigned short col1,  unsigned short col2);
    void setPalette8(int x, int y, int pal);

    int players() const;
    void players(int );

    void drawGfx8(
        const unsigned short * gfx,
        const unsigned short * map,
        int x, int y, int frame
        );

  private:
    nds::Background * m_bg;
    int m_playerCount;
    
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

    Arena();
    ~Arena();
    void clearSquare(int x, int y);
};

#endif
