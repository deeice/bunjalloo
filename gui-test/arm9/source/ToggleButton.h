#ifndef ToggleButton_h_seen
#define ToggleButton_h_seen

#include "ButtonI.h"
#include "Component.h"

class ToggleButton: public Component, public ButtonI
{
  public:
    ToggleButton();

    void paint(const nds::Rectangle & clip);
    bool touch(int x, int y);

  protected:
    virtual const unsigned char *getGfxData() = 0;
    virtual const unsigned short *getPaletteData() = 0;

  private:
};
#endif
