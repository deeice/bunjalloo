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
  SPRITE_EDIT,
  SPRITE_ADD,
  SPRITE_END_OF_ENTRIES,
};

PreferencesToolbar::PreferencesToolbar(Document & doc, Controller & cont, View & view) :
  Toolbar(doc, cont, view, SPRITE_END_OF_ENTRIES)
{
}

void PreferencesToolbar::updateIcons()
{
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ICON_BACK);
  m_sprites[SPRITE_EDIT]->setTile( TILES_PER_ICON * ICON_COOKIE);
  m_sprites[SPRITE_ADD]->setTile( TILES_PER_ICON * ICON_ADD_COOKIE);
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
    case (SPRITE_ADD):
      break;
    default:
      break;
  }
}

