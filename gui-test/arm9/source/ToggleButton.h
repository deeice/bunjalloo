#ifndef ToggleButton_h_seen
#define ToggleButton_h_seen

#include "ButtonI.h"
#include "Component.h"

/** A dual state button that toggles on and off as the user presses it.*/
class ToggleButton: public Component, public ButtonI
{
  public:
    /** Construct a new ToggleButton.*/
    ToggleButton();

    // from Component
    virtual void paint(const nds::Rectangle & clip);
    virtual bool touch(int x, int y);

  protected:
    /** Concrete classes that inherit from ToggleButton should return the
     * graphics to display.
     * @return the graphics data, in "sprite base" tile format. The implementer
     * should return the correct data for selected/unselected state by checking
     * selected()
     */
    virtual const unsigned char *getGfxData() = 0;

    /** Concrete classes that inherit from ToggleButton should return the
     * palette for the displayed graphics.
     * @return the palette data that corresponds to the value returned by getGfxData()
     */
    virtual const unsigned short *getPaletteData() = 0;

  private:
};
#endif
