#ifndef ScreenI_h_seen
#define ScreenI_h_seen

//! A nasty enum of the current screen value.
enum CurrentScreen_t {
  SCR_SPLASH,
  SCR_OPTIONS,
  SCR_CREATE_PLAYERS,
  SCR_CREATE_LOBBY,
  SCR_EDIT_NAME,
  SCR_GAME_MENU,
  SCR_SELECT_SPELL,
  SCR_EXAMINE_SPELL,
  SCR_EXAMINE_BOARD,

  SCR_CASTING,
  SCR_MOVEMENT,
  SCR_INFO_SCREEN,
  SCR_EXAMINE
};
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
    //! Fetch the screen identifier
    virtual CurrentScreen_t screenId() const = 0;
    //! Handle keypad presses
    virtual void handleKeys()=0;
};
#endif
