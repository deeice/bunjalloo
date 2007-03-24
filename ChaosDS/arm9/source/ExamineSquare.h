#ifndef ExamineSquare_h_seen
#define ExamineSquare_h_seen
#include "ScreenI.h"

class ExamineSquare: public ScreenI
{
  public:
    /** Print a statistic with its name and value. For example "CAST CHANCE = 10%".
     * @param value The value of the statistic.
     * @param index The index of the statistic in the table.
     * @param palette The palette of the statistic name.
     * @param palette2 The palette of the statisitic value.
     * @param percent If true then a % sign is drawn too.
     */
    static void printStat(int value,
                          int index,
                          int palette, 
                          int palette2,
                          bool percent=false);
    /** Draw all stats for a given thing.
     * Uses pointer arithmetic to offset into the things data.
     * @param p pointer to a spell or wizard to print.
     */
    static void drawStats(const unsigned char * p);

    /*! @brief Create the examine screen, passing in the screen that should be
     * shown when this one ends.
     * @param returnScreen screen to show when the user exits this one.
     */
    ExamineSquare(ScreenI * returnScreen);
    virtual void show();
    virtual void animate();
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
