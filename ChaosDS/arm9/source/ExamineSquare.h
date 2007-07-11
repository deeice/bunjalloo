#ifndef ExamineSquare_h_seen
#define ExamineSquare_h_seen
//#include "TouchScreen.h"
#include "ScreenI.h"

class Text16;

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

    /*! @brief Create the examine screen
     */
    ExamineSquare();
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
    
    //! Hide the examine screen.
    void hide();

    /** Get the current hidden status.
     * @return true if hidden, false if shown.
     */
    bool isHidden() const;
    
    //! Set whether the cast chance should be shown or not.
    void showCastChance(bool castChance=true);

    /** Get the arena index that is being examined.
     * @return the arena index that is examined.
     */
    int index() const;

    void initialise();

  private:
    int m_creature;
    int m_underneath;
    int m_flags;
    int m_index;
    bool m_first;
    bool m_showCastChance; //!< Show the cast chance too. False by default.
    Text16 * m_text;
    int m_counter;
    bool m_shown;
    static void exitCb(void * arg);

    void displayCreatureData(int creature, int underneath, int flags);
    void displaySpellData(int creature);
    void next();
};
#endif
