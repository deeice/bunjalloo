/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
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
