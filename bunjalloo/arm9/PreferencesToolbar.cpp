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
      break;
    case (SPRITE_EDIT_CONFIG):
      m_view.editConfig();
      break;
    default:
      break;
  }
}

