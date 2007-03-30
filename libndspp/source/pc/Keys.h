#ifndef Keys_h_seen
#define Keys_h_seen

struct SDL_KeyboardEvent;

#include <SDL/SDL_keyboard.h>

class Keys
{
  public:
    static Keys & instance();
    void handleKeyEvent(SDL_KeyboardEvent & event);
    void scan();
    int  down() const;
    int held();
    int repeat();
    void setRepeat(int setDelay, int setRepeat);

  private:
    static const int KEY_COUNT = 12;
    static const SDLKey s_keyCodes[Keys::KEY_COUNT];

    unsigned int m_held;
    unsigned int m_heldLast;
    unsigned int m_heldRepeat;

    unsigned int m_repeat;
    unsigned int m_delay;
    unsigned int m_count;

    bool m_keys[KEY_COUNT];

    unsigned int keyRegister();

    Keys();

};
#endif
