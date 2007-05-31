#ifndef ArenaTouchScreen_h_seen
#define ArenaTouchScreen_h_seen

#include "TouchScreen.h"
class ArenaTouchScreen: public TouchScreen
{

  public:
    ArenaTouchScreen();

    /** Examine the current square.  */
    virtual void examine() = 0;

    /** Execute action on the current square (selected twice) */
    virtual void execute() = 0;

    /** Next action button pressed. */
    virtual void next() = 0;

  private:
    static void examineCb(void * arg);
    static void exitCb(void * arg);
    static void arenaCb(void * arg);

};
#endif
