#ifndef RadioButton_h_seen
#define RadioButton_h_seen

#include "ToggleButton.h"
/** A typically round button that the user can toggle on or off. */
class RadioButton : public ToggleButton
{
  public:
    /** Sets up the new RadioButton.*/
    RadioButton();

  protected:
    virtual const unsigned char * getGfxData();
    virtual const unsigned short * getPaletteData();

};
#endif
