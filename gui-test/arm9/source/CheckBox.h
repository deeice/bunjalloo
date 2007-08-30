#ifndef CheckBox_h_seen
#define CheckBox_h_seen

#include "ToggleButton.h"
class CheckBox : public ToggleButton
{
  public:
    CheckBox();

  protected:
    const unsigned char * getGfxData();
    const unsigned short * getPaletteData();

};
#endif
