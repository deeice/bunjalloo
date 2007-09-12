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
*/
#include "libnds.h"
#include "images.h"
#include "GfxData.h"

const GfxData s_paletteData[10] = {
  {_binary_palette0_pal_start, (u32)_binary_palette0_pal_size},
  {_binary_palette1_pal_start, (u32)_binary_palette1_pal_size},
  {_binary_palette2_pal_start, (u32)_binary_palette2_pal_size},
  {_binary_palette3_pal_start, (u32)_binary_palette3_pal_size},
  {_binary_palette4_pal_start, (u32)_binary_palette4_pal_size},
  {_binary_palette5_pal_start, (u32)_binary_palette5_pal_size},
  {_binary_palette6_pal_start, (u32)_binary_palette6_pal_size},
  {_binary_palette7_pal_start, (u32)_binary_palette7_pal_size},
  {_binary_palette8_pal_start, (u32)_binary_palette8_pal_size},
  {_binary_palette9_pal_start, (u32)_binary_palette9_pal_size},
};

