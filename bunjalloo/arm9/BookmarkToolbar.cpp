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
#include "BookmarkToolbar.h"
#include "Sprite.h"
#include "View.h"

using nds::Sprite;

enum ToolbarSpriteID
{
  SPRITE_BACK,
  SPRITE_ADD,
  SPRITE_END_OF_ENTRIES,
};

BookmarkToolbar::BookmarkToolbar(View & view) :
  Toolbar(view, SPRITE_END_OF_ENTRIES)
{
}

void BookmarkToolbar::updateIcons()
{
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ICON_BACK);
  m_sprites[SPRITE_ADD]->setTile( TILES_PER_ICON * ICON_ADD_BOOKMARK);
}

void BookmarkToolbar::tick()
{}

void BookmarkToolbar::handlePress(int i)
{
  switch ((ToolbarSpriteID)i)
  {
    case (SPRITE_BACK):
      m_view.endBookmark();
      break;
    case (SPRITE_ADD):
      m_view.bookmarkCurrentPage();
      break;
    default:
      break;
  }
}
void BookmarkToolbar::layout()
{
  Toolbar::layout();
}

