#ifndef ScreenI_h_seen
#define ScreenI_h_seen

//! Interface for screens.
class ScreenI
{
  public:
    //! Destructor is required since the base class is abstract.
    virtual ~ScreenI() {};
    //! Called when the screen is to be shown
    virtual void show() = 0;
    //! Perform any animation tasks.
    virtual void animate() = 0;
    //! Vblank handler for the screen
    virtual void vblank() {}
    //! Handle keypad presses
    virtual void handleKeys()=0;
    
};
#endif
