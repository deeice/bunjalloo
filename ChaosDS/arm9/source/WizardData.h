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
#ifndef WizardData_h_seen
#define WizardData_h_seen

typedef struct 
{
  const unsigned short * const gfx; //!< gfx data (tile data)
  const unsigned short * const map; //!< map data
} WizardData_t;

enum {
   GFX_MAGIC_SHIELD = 8,
   GFX_MAGIC_ARMOUR,
   GFX_MAGIC_SWORD,
   GFX_MAGIC_KNIFE,
   GFX_MAGIC_BOW,
   GFX_MAGIC_WINGS,
};

extern const WizardData_t s_wizardData[];
#endif
