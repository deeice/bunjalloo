#ifndef CheckBox_h_seen
#define CheckBox_h_seen

#include "ToggleButton.h"
/** A CheckBox is a button that can be selected or deselected, toggling its
 * state each time it is pressed.*/
class CheckBox : public ToggleButton
{
  public:
    /** Create an instance of this class. */
    CheckBox();

  protected:
    virtual const unsigned char * getGfxData();
    virtual const unsigned short * getPaletteData();

};
#endif
