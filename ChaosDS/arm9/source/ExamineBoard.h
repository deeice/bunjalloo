#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ScreenI.h"

class ExamineBoard: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual void vblank();
    virtual void handleKeys();

    ~ExamineBoard();

  private:

    void a();
    void b();
};
#endif
