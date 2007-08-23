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
#include "images.h"
#include "WizardData.h"

const WizardData_t s_wizardData[] = {
   {_binary_wiz1_raw_start, _binary_wiz1_map_start},
   {_binary_wiz2_raw_start, _binary_wiz2_map_start},
   {_binary_wiz3_raw_start, _binary_wiz3_map_start},
   {_binary_wiz4_raw_start, _binary_wiz4_map_start},
   {_binary_wiz5_raw_start, _binary_wiz5_map_start},
   {_binary_wiz6_raw_start, _binary_wiz6_map_start},
   {_binary_wiz7_raw_start, _binary_wiz7_map_start},
   {_binary_wiz8_raw_start, _binary_wiz8_map_start},
  
   {_binary_shield_raw_start, _binary_shield_map_start},
   {_binary_armour_raw_start, _binary_armour_map_start},
   {_binary_sword_raw_start,  _binary_sword_map_start},
   {_binary_knife_raw_start,  _binary_knife_map_start},
   {_binary_bow_raw_start,    _binary_bow_map_start},
   {_binary_wings_raw_start,  _binary_wings_map_start},
};

