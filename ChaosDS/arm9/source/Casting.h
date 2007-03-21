#ifndef Casting_h_seen
#define Casting_h_seen

#include "ScreenI.h"

class Casting: public ScreenI
{
  public:
    /** initialise the object. 
     * @param start is it a start screen or returning.
     */
    Casting(bool start=true);

    //! get the current global chaos value
    static int worldChaos();
    //! set the global chaos value to 0.
    static void resetWorldChaos();
    //! set the spell success indicator to the given value.
    static void setSpellSuccess(bool success);
    /** Calculate if the spell succeeds, using the spell cast chance, illusion
     * flag and random factors.
     */
    static bool calculateSpellSuccess();
    //! @return true if the spell succeeds.
    static bool spellSuccess();
    //! Do the spell animation from the start index ot the target index.
    static void spellAnimation();
    //! Print the outcome of the spell cast.
    static void printSuccessStatus();
    
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();
  private:
    static int s_worldChaos;
    static bool s_castSuccess;
    bool m_start;
    void startCastRound();
    void execute();
    void cancel();
    void nextPlayerCast();
    void examineSquare();
    
};
#endif
