#ifndef EditName_h_seen
#define EditName_h_seen
#include "ScreenI.h"
class EditName : public ScreenI
{
  public:
    virtual void show();
    virtual void animate();
    virtual void handleKeys();
  private:
    int m_hilightItem;
    char m_name[12];

    void updateName();
    void selectLetter();
    void deselectLetter();
    void indexToPosition(int & x, int &y, char & c);
    void up();
    void down();
    void left();
    void right();
    void a();
    void b();
    void start();
};
#endif
