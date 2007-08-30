#ifndef RadioButton_h_seen
#define RadioButton_h_seen

#include "ToggleButton.h"
class RadioButton : public ToggleButton
{
  public:
    RadioButton();

  protected:
    const unsigned char * getGfxData();
    const unsigned short * getPaletteData();

};
#endif
