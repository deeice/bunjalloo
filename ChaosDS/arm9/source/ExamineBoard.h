#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ArenaTouchScreen.h"

class ExamineBoard: public ArenaTouchScreen
{
  public:
    /** Show the examine board screen.
     * @param start true if this is the first time calling. false if returning (e.g. after examine square)
     */
    ExamineBoard(bool start=true);
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();

    // from ArenaTouchScreen
    virtual void next();
    virtual void execute();
    virtual void examine();

  private:
    bool m_start;

};
#endif
