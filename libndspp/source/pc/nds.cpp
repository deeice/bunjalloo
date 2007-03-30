#include "libnds.h"
#include "SDL/SDL.h"
#include <string.h>
#include <iostream>
#include "SDLhandler.h"

void dmaCopy(const void * source, void * destination, unsigned int length)
{
  ::memcpy(destination, source, length);
}
using namespace std;

void powerON()
{
}
void irqInit()
{
}
void irqSet(int, VoidFunctionPointer fn)
{
  SDLhandler::instance().enableVblank(fn);
}

void swiWaitForVBlank() 
{
  SDLhandler::instance().waitVsync();
}

#if 0
void glReset(void)
{
  SDL_GL_SwapBuffers( );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity( );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity( );

}
#endif
