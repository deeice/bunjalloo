#include "libnds.h"
#include "SDLhandler.h"

void setGenericSound( u32 rate, u8 vol, u8 pan, u8 format)
{
  int sdlFormat(format==0?AUDIO_S16:AUDIO_S8);
  SDLhandler::instance().initSound(rate, sdlFormat);
}

void playGenericSound(const void* data, u32 length)
{
  SDLhandler::instance().playSound(data, length);
}


