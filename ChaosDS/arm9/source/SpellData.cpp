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
#include "magic.h"
#include "SpellData.h"
const SpellData s_spellData[] = {
  {"None", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cast_disbelieve, 0, 0, 0, },
  {"Disbelieve", 0, 10, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, cast_disbelieve, 0, 0, 0, },
  {"Meditate", 0, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, cast_meditate, NEW_FEATURE, 0, 0, },
  {"King Cobra", 1, 10, 3, 18, 0, 4, 0, 0, 1, 1, 6, 1, cast_creature, 0, king_cobraTiles, king_cobraMap, },
  {"Dire Wolf", -1, 9, 3, 18, 1, 3, 0, 0, 2, 3, 7, 2, cast_creature, 0, dire_wolfTiles, dire_wolfMap, },
  {"Goblin", -1, 9, 3, 18, 6, 2, 0, 0, 4, 1, 4, 4, cast_creature, 0, goblinTiles, goblinMap, },
  {"Crocodile", 0, 8, 3, 18, 0, 5, 0, 0, 6, 1, 2, 2, cast_creature, 0, crocodileTiles, crocodileMap, },
  {"Troll", -1, 6, 3, 18, 1, 4, 0, 0, 8, 1, 1, 4, cast_creature, NEW_FEATURE, trollTiles, trollMap, },
  {"Faun", -1, 8, 3, 18, 3, 3, 0, 0, 2, 1, 7, 8, cast_creature, 0, faunTiles, faunMap, },
  {"Lion", 1, 6, 3, 18, 2, 6, 0, 0, 4, 4, 8, 3, cast_creature, 0, lionTiles, lionMap, },
  {"Elf", 2, 7, 3, 18, 0, 1, 2, 6, 2, 1, 5, 7, cast_creature, 0, elfTiles, elfMap, },
  {"Orc", -1, 10, 3, 18, 3, 2, 0, 0, 1, 1, 4, 4, cast_creature, 0, orcTiles, orcMap, },
  {"Bear", 1, 6, 3, 18, 3, 6, 0, 0, 7, 2, 6, 2, cast_creature, 0, bearTiles, bearMap, },
  {"Gorilla", 0, 7, 3, 18, 7, 6, 0, 0, 5, 1, 4, 2, cast_creature, 0, gorillaTiles, gorillaMap, },
  {"Ogre", -1, 7, 3, 18, 4, 4, 0, 0, 7, 1, 3, 6, cast_creature, 0, ogreTiles, ogreMap, },
  {"Hydra", -1, 5, 3, 18, 0, 7, 0, 0, 8, 1, 4, 6, cast_creature, 0, hydraTiles, hydraMap, },
  {"Giant Rat", 0, 10, 3, 18, 1, 1, 0, 0, 1, 3, 8, 2, cast_creature, 0, giant_ratTiles, giant_ratMap, },
  {"Giant", 1, 4, 3, 18, 6, 9, 0, 0, 7, 2, 6, 5, cast_creature, 0, giantTiles, giantMap, },
  {"Horse", 1, 9, 3, 18, 3, 1, 0, 0, 3, 4, 8, 1, cast_creature, 0, horseTiles, horseMap, },
  {"Unicorn", 2, 6, 3, 18, 7, 5, 0, 0, 4, 4, 9, 7, cast_creature, 0, unicornTiles, unicornMap, },
  {"Centaur", 1, 7, 3, 18, 3, 1, 2, 4, 3, 4, 5, 5, cast_creature, 0, centaurTiles, centaurMap, },
  {"Pegasus", 2, 7, 3, 18, 1, 2, 0, 0, 4, 5, 6, 7, cast_creature, 0, pegasusTiles, pegasusMap, },
  {"Gryphon", 1, 6, 3, 18, 1, 3, 0, 0, 5, 5, 5, 6, cast_creature, 0, gryphonTiles, gryphonMap, },
  {"Manticore", -1, 4, 3, 18, 2, 3, 1, 3, 6, 5, 6, 8, cast_creature, 0, manticoreTiles, manticoreMap, },
  {"Bat", -1, 8, 3, 18, 1, 1, 0, 0, 1, 5, 9, 4, cast_creature, 0, batTiles, batMap, },
  {"Green Dragon", -1, 2, 3, 18, 0, 5, 4, 6, 8, 3, 4, 4, cast_creature, 0, green_dragonTiles, green_dragonMap, },
  {"Red Dragon", -2, 1, 3, 18, 4, 7, 3, 5, 9, 3, 4, 5, cast_creature, 0, red_dragonTiles, red_dragonMap, },
  {"Golden Dragon", 2, 2, 3, 18, 2, 9, 5, 4, 9, 3, 5, 5, cast_creature, 0, golden_dragonTiles, golden_dragonMap, },
  {"Harpy", -1, 7, 3, 18, 1, 4, 0, 0, 2, 5, 8, 5, cast_creature, 0, harpyTiles, harpyMap, },
  {"Eagle", 1, 7, 3, 18, 2, 3, 0, 0, 3, 6, 8, 2, cast_creature, 0, eagleTiles, eagleMap, },
  {"Vampire", -2, 2, 3, 18, 1, 6, 0, 0, 8, 4, 6, 5, cast_creature, 0, vampireTiles, vampireMap, },
  {"Ghost", -1, 5, 3, 18, 4, 1, 0, 0, 3, 2, 9, 6, cast_creature, 0, ghostTiles, ghostMap, },
  {"Spectre", -1, 6, 3, 18, 7, 4, 0, 0, 2, 1, 6, 4, cast_creature, 0, spectreTiles, spectreMap, },
  {"Wraith", -1, 6, 3, 18, 5, 5, 0, 0, 5, 2, 4, 5, cast_creature, 0, wraithTiles, wraithMap, },
  {"Skeleton", -1, 7, 3, 18, 4, 3, 0, 0, 2, 1, 3, 4, cast_creature, 0, skeletonTiles, skeletonMap, },
  {"Zombie", -1, 9, 3, 18, 7, 1, 0, 0, 1, 1, 2, 3, cast_creature, 0, zombieTiles, zombieMap, },
  {"Gooey Blob", -1, 9, 13, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_fire_goo, 0, gooey_blobTiles, gooey_blobMap, },
  {"Magic Fire", -1, 8, 13, 18, 4, 0, 0, 0, 0, 0, 0, 0, cast_fire_goo, 0, magic_fireTiles, magic_fireMap, },
  {"Magic Wood", 1, 8, 17, 17, 8, 0, 0, 0, 0, 0, 0, 0, cast_trees_castles, 0, magic_woodTiles, magic_woodMap, },
  {"Shadow Wood", -1, 4, 17, 18, 5, 0, 0, 0, 0, 0, 0, 0, cast_trees_castles, 0, shadow_woodTiles, shadow_woodMap, },
  {"Magic Castle", 1, 6, 17, 23, 5, 0, 0, 0, 0, 0, 0, 0, cast_trees_castles, 0, magic_castleTiles, magic_castleMap, },
  {"Dark Citadel", -1, 6, 17, 23, 5, 0, 0, 0, 0, 0, 0, 0, cast_trees_castles, 0, dark_citadelTiles, dark_citadelMap, },
  {"Wall", 0, 8, 13, 12, 3, 0, 0, 0, 0, 0, 0, 0, cast_wall, 0, wallTiles, wallMap, },
  {"Magic Bolt", 0, 10, 13, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_missile, 0, 0, 0, },
  {"Blind", -1, 10, 13, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_missile, NEW_FEATURE, 0, 0, },
  {"Lightning", 0, 10, 9, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_missile, 0, 0, 0, },
  {"Magic Sleep", 1, 10, 13, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_missile, NEW_FEATURE, 0, 0, },
  {"Vengeance", -1, 8, 41, 12, 0, 0, 0, 0, 0, 0, 0, 0, cast_justice, 0, 0, 0, },
  {"Decree", 1, 8, 41, 12, 0, 0, 0, 0, 0, 0, 0, 0, cast_justice, 0, 0, 0, },
  {"Dark Power", -2, 5, 41, 11, 0, 0, 0, 0, 0, 0, 0, 0, cast_justice, 0, 0, 0, },
  {"Justice", 2, 5, 41, 11, 0, 0, 0, 0, 0, 0, 0, 0, cast_justice, 0, 0, 0, },
  {"Magic Shield", 1, 7, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_shield, 0, 0, 0, },
  {"Magic Armour", 1, 4, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_armour, 0, 0, 0, },
  {"Magic Sword", 1, 4, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_sword, 0, 0, 0, },
  {"Magic Knife", 1, 7, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_knife, 0, 0, 0, },
  {"Magic Bow", 1, 5, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_bow, 0, 0, 0, },
  {"Magic Wings", 0, 5, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, cast_magic_wings, 0, 0, 0, },
  {"Law 1", 2, 8, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, cast_chaos_law, 0, 0, 0, },
  {"Law 2", 4, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, cast_chaos_law, 0, 0, 0, },
  {"Chaos 1", -2, 8, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, cast_chaos_law, 0, 0, 0, },
  {"Chaos 2", -4, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, cast_chaos_law, 0, 0, 0, },
  {"Shadow Form", 0, 8, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_shadow_form, 0, 0, 0, },
  {"Subversion", 0, 10, 15, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_subversion, 0, 0, 0, },
  {"Mutation", 0, 10, 15, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_subversion, NEW_FEATURE, 0, 0, },
  {"Raise Dead", -1, 5, 9, 18, 0, 0, 0, 0, 0, 0, 0, 0, cast_raise_dead, 0, 0, 0, },
  {"Turmoil", -1, 5, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, cast_turmoil, 0, 0, 0, },
};
