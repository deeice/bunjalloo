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
*/
#include <algorithm>
#include <functional>
#include <cstdio>
#include "PreferencesToolbar.h"
#include "Sprite.h"
#include "View.h"

using nds::Sprite;

enum ToolbarSpriteID
{
  SPRITE_BACK,
  SPRITE_EDIT_COOKIES,
  SPRITE_ADD_COOKIE,
  SPRITE_EDIT_SEARCH,
  SPRITE_ADD_SEARCH,
  SPRITE_EDIT_CONFIG,
  SPRITE_END_OF_ENTRIES
};

PreferencesToolbar::PreferencesToolbar(View & view) :
  Toolbar(view, SPRITE_END_OF_ENTRIES)
{
}

void PreferencesToolbar::updateIcons()
{
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ICON_BACK);
  m_sprites[SPRITE_EDIT_COOKIES]->setTile( TILES_PER_ICON * ICON_COOKIE);
  m_sprites[SPRITE_ADD_COOKIE]->setTile( TILES_PER_ICON * ICON_ADD_COOKIE);
  m_sprites[SPRITE_EDIT_SEARCH]->setTile( TILES_PER_ICON * ICON_SEARCH);
  m_sprites[SPRITE_ADD_SEARCH]->setTile( TILES_PER_ICON * ICON_ADD_SEARCH);
  m_sprites[SPRITE_EDIT_CONFIG]->setTile( TILES_PER_ICON * ICON_PREFS);
}

void PreferencesToolbar::tick()
{}

void PreferencesToolbar::handlePress(int i)
{
  switch ((ToolbarSpriteID)i)
  {
    case (SPRITE_BACK):
      m_view.endBookmark();
      break;
    case (SPRITE_ADD_COOKIE):
      m_view.addCookie();
      break;
    case (SPRITE_EDIT_COOKIES):
      m_view.editCookie();
      break;
    case (SPRITE_EDIT_CONFIG):
      m_view.editConfig();
      break;
    default:
      break;
  }
}

