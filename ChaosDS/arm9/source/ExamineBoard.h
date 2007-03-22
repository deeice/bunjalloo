#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ScreenI.h"

class ExamineBoard: public ScreenI
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

  private:
    bool m_start;

    void a();
    void b();
};
#endif
