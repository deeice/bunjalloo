#ifndef ButtonListener_h_seen
#define ButtonListener_h_seen

class ButtonI;
/** Classes that want to register interest in button presses should inherit
 * from this interface.*/
class ButtonListener
{
  public:
    
    /** End of lifetime. */
    virtual ~ButtonListener() {};

    /** Callback for when a ButtonI is pressed. 
     * @param button typically this is the button that was pressed.
     */
    virtual void pressed(ButtonI * button) = 0;
};
#endif
