#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ScreenI.h"

class ExamineBoard: public ScreenI
{
  public:
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
