#ifndef ScreenI_h_seen
#define ScreenI_h_seen

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
// interface for screens...
class ScreenI
{
  public:
    virtual ~ScreenI() {};
    virtual void show() = 0;
    virtual void animate() = 0;
    virtual CurrentScreen_t getId() const = 0;

    virtual void handleKeys()=0;
};
#endif
