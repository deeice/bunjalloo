#include "libnds.h"
#include "Keys.h"
#include "SDL/SDL.h"

const SDLKey Keys::s_keyCodes[Keys::KEY_COUNT] =
{
  SDLK_z,      // KEY_A 	  
  SDLK_x,      // KEY_B 	  
  SDLK_SPACE,  // KEY_SELECT
  SDLK_RETURN, // KEY_START 
  SDLK_RIGHT,  // KEY_RIGHT 
  SDLK_LEFT,   // KEY_LEFT  
  SDLK_UP,     // KEY_UP 	  
  SDLK_DOWN,   // KEY_DOWN  
  SDLK_w,      // KEY_R	  
  SDLK_q,      // KEY_L 	  
  SDLK_a,      // KEY_X
  SDLK_s,      // KEY_Y
};

Keys::Keys():
    m_repeat(30),
    m_delay(60),
    m_count(60)
{}

Keys & Keys::instance()
{
  static Keys s_instance;
  return s_instance;
}

void Keys::handleKeyEvent(SDL_KeyboardEvent & event)
{
  if (event.type == SDL_KEYDOWN)
  {
    // set in key array  
    for (int i = 0; i < KEY_COUNT; i++) {
      if (s_keyCodes[i] == event.keysym.sym) {
        m_keys[i] = true;
      }
    }
  }
  else if (event.type == SDL_KEYUP)
  {  
    // unset in key array
    for (int i = 0; i < KEY_COUNT; i++) {
      if (s_keyCodes[i] == event.keysym.sym) {
        m_keys[i] = false;
      }
    }
  }
}

unsigned int Keys::keyRegister()
{
  unsigned int keyReg = 0;
  for (int i = 0; i < KEY_COUNT; ++i)
  {
    if (m_keys[i]) {
      keyReg |= 1<<i;
    }
  }
  return keyReg;
}

void Keys::scan()
{
  m_heldLast = m_held;
  m_held = keyRegister();

  if ( m_delay != 0 )
  {
    if (m_held != m_heldLast) 
    {
      m_count = m_delay;
      m_heldRepeat = down();
    }
    
    m_count--;
    
    if (m_count == 0) 
    {
      m_count = m_repeat;
      m_heldRepeat = m_held;
    }
  }
}

int  Keys::down() const
{
  return (m_held ^ m_heldLast) & m_held;
}

int Keys::held()
{
  return m_held;
}

int Keys::repeat()
{
  int tmp = m_heldRepeat;
  m_heldRepeat = 0;
  return tmp;
}

void Keys::setRepeat(int setDelay, int setRepeat)
{
  m_delay  = setDelay;
  m_repeat = setRepeat;
  m_count  = setDelay;
  m_heldRepeat = 0;
}

// static functions that emulate libnds
void scanKeys()
{
  Keys::instance().scan();
}
u32 keysDown()
{
  return Keys::instance().down();
}
u32 keysHeld()
{
  return Keys::instance().held();
}
u32 keysDownRepeat()
{
  return Keys::instance().repeat();
}
void keysSetRepeat( u8 setDelay, u8 setRepeat )
{
  Keys::instance().setRepeat(setDelay, setRepeat);
}
