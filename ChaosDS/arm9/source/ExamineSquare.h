#ifndef ExamineSquare_h_seen
#define ExamineSquare_h_seen
#include "ScreenI.h"

class ExamineSquare: public ScreenI
{
  public:
    static void printStat(int value, int index, int palette, 
        int palette2, bool percent=false);
    static void drawStats(const unsigned char *);

    /*! @brief Create the examine screen, passing in the screen that should be
     * shown when this one ends.
     * @param returnScreen screen to show when the user exits this one.
     */
    ExamineSquare(ScreenI * returnScreen);
    ~ExamineSquare();
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
    
    //! Set whether the cast chance should be shown or not.
    void showCastChance(bool castChance=true);

  private:
    int m_creature;
    int m_underneath;
    int m_flags;
    bool m_first;
    bool m_showCastChance; //!< Show the cast chance too. False by default.
    ScreenI * m_returnScreen;

    void displayCreatureData(int creature, int underneath, int flags);
    void displaySpellData(int creature);
};
#endif
