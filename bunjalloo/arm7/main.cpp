/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  This file incorporates work covered by the following copyright and  
  permission notice:  

     Copyright (C) 2005
             Michael Noland (joat)
             Jason Rogers (dovoto)
             Dave Murphy (WinterMute)

     This software is provided 'as-is', without any express or implied
     warranty.  In no event will the authors be held liable for any
     damages arising from the use of this software.

     Permission is granted to anyone to use this software for any
     purpose, including commercial applications, and to alter it and
     redistribute it freely, subject to the following restrictions:

     1.      The origin of this software must not be misrepresented; you
             must not claim that you wrote the original software. If you use
             this software in a product, an acknowledgment in the product
             documentation would be appreciated but is not required.
     2.      Altered source versions must be plainly marked as such, and
             must not be misrepresented as being the original software.
     3.      This notice may not be removed or altered from any source
             distribution.

*/

#include <nds.h>
#include <stdlib.h>
#include <dswifi7.h>
#include "Wifi7.h"

using namespace nds;

void startSound(int sampleRate, const void* data, u32 bytes, u8 channel, u8 vol,  u8 pan, u8 format) {
  SCHANNEL_TIMER(channel)  = SOUND_FREQ(sampleRate);
  SCHANNEL_SOURCE(channel) = (u32)data;
  SCHANNEL_LENGTH(channel) = bytes >> 2 ;
  SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | (format==1?SOUND_8BIT:SOUND_16BIT);
}

s32 getFreeSoundChannel() {
  int i;
  for (i=0; i<16; i++) {
    if ( (SCHANNEL_CR(i) & SCHANNEL_ENABLE) == 0 ) return i;
  }
  return -1;
}

touchPosition tempPos;

void VcountHandler() {
  static int lastbut = -1;

  uint16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0;

  but = REG_KEYXY;

  if (!( (but ^ lastbut) & (1<<6))) {

    tempPos = touchReadXY();

    if ( tempPos.x == 0 || tempPos.y == 0 ) {
      but |= (1 <<6);
      lastbut = but;
    } else {
      x = tempPos.x;
      y = tempPos.y;
      xpx = tempPos.px;
      ypx = tempPos.py;
      z1 = tempPos.z1;
      z2 = tempPos.z2;
    }

  } else {
    lastbut = but;
    but |= (1 <<6);
  }

  IPC->touchX   = x;
  IPC->touchY   = y;
  IPC->touchXpx = xpx;
  IPC->touchYpx = ypx;
  IPC->touchZ1  = z1;
  IPC->touchZ2  = z2;
  IPC->buttons  = but;

}

void VblankHandler(void) {
  u32 i;
  //sound code  :)
  TransferSound *snd = IPC->soundData;
  IPC->soundData = 0;

  if (0 != snd) {

    for (i=0; i < snd->count; i++) {
      s32 chan = getFreeSoundChannel();

      if (chan >= 0) {
        startSound(snd->data[i].rate, 
            snd->data[i].data, 
            snd->data[i].len, 
            chan, 
            snd->data[i].vol, 
            snd->data[i].pan, 
            snd->data[i].format);
      }
    }
  }

}

void WifiVblankHandler()
{
  // default vblank handler
  VblankHandler();
  // update wireless in vblank
  Wifi_Update();
}

int main(int argc, char ** argv) {

  // must be the first thing we do...
  Wifi7 wifi;
  irqInit();

  // Start the RTC tracking IRQ
  initClockIRQ();

  IPC->soundData = 0;

  irqSet(IRQ_VBLANK, WifiVblankHandler);
  irqEnable(IRQ_VBLANK);
  SetYtrigger(80);
  irqSet(IRQ_VCOUNT, VcountHandler);
  irqEnable((IRQ_MASK)(IRQ_VBLANK | IRQ_VCOUNT));

  // enter the main loop - handles wifi shutdown and restart
  wifi.mainLoop();

}

