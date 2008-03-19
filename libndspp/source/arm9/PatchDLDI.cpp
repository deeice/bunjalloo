/*
  Copyright (C) 2008 Richard Quirk

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

    dlditool - Dynamically Linked Disk Interface patch tool
    Copyright (C) 2006  Michael Chisholm (Chishm)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*/
#include <nds.h>
#include <nds/arm9/video.h>
#include <string.h>
#include "PatchDLDI.h"

extern "C"
{
  extern const u32 _io_dldi;
}

const unsigned char * nds::PatchDLDI::dldiPatch()
{
  return (unsigned char*)(((unsigned int*)(&_io_dldi)) -24);
}

unsigned short * nds::PatchDLDI::buffer()
{
  VRAM_C_CR = VRAM_ENABLE | VRAM_C_LCD;
  // Clear VRAM
  memset(VRAM_C, 0x00, 128 * 1024);
  return VRAM_C;
}
