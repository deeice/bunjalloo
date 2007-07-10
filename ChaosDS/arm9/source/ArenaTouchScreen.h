#ifndef ArenaTouchScreen_h_seen
#define ArenaTouchScreen_h_seen

#include "TouchScreen.h"
class ExamineSquare;
class ArenaTouchScreen: public TouchScreen
{

  public:
    ArenaTouchScreen();
    ~ArenaTouchScreen();

    /** Examine the current square.  */
    void examine();

    /** Execute action on the current square (selected twice) */
    virtual void execute() = 0;

    /** Next action button pressed. */
    virtual void next() = 0;

  private:
    static void examineCb(void * arg);
    static void exitCb(void * arg);
    static void arenaCb(void * arg);

    void deleteScreen();

  protected:
    ExamineSquare * m_examineScreen;

};
#endif
