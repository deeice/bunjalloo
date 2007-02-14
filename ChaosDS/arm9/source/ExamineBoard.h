#ifndef ExamineBoard_h_seen
#define ExamineBoard_h_seen
#include "ScreenI.h"

class ExamineBoard: public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual CurrentScreen_t getId() const;
    virtual void handleKeys();
};
#endif
