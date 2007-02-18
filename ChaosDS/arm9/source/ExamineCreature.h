#ifndef ExamineCreature_h_seen
#define ExamineCreature_h_seen
#include "ScreenI.h"

class ExamineCreature: public ScreenI
{
  public:
    ExamineCreature(ScreenI *);
    ~ExamineCreature();
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();

    void showCastChance(bool castChance=true);

    static void printStat(int value, int index, int palette, 
        int palette2, bool percent=false);

    static void drawStats(const unsigned char *);

  private:
    int m_creature;
    int m_underneath;
    int m_flags;
    bool m_first;
    bool m_showCastChance; //!< Show the cast chance too. False by default.
    ScreenI * m_returnScreen;

    void displayData(int creature, int underneath, int flags);
};
#endif
