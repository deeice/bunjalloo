#ifndef Casting_h_seen
#define Casting_h_seen

#include "ScreenI.h"

class Casting: public ScreenI
{
  public:
    //! get the current global chaos value
    static int getWorldChaos();
    static void setSpellSuccess();
    
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
  private:
    static int s_worldChaos;
    static bool s_castSuccess;
    void startCastRound();
    void execute();
    void cancel();
    void nextPlayerCast();
    void printSuccessStatus();
    
};
#endif
