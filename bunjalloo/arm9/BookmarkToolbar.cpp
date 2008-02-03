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

BookmarkToolbar::BookmarkToolbar(Document & doc, Controller & cont, View & view) :
  Toolbar(doc, cont, view, SPRITE_END_OF_ENTRIES)
{
}

void BookmarkToolbar::updateIcons()
{
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ICON_BACK);
  m_sprites[SPRITE_ADD]->setTile( TILES_PER_ICON * ICON_ADD_BOOKMARK);
}

void BookmarkToolbar::tick()
{}

void BookmarkToolbar::setVisible(bool visible)
{
  Toolbar::setVisible(visible);
  for_each(m_sprites.begin(), m_sprites.end(), std::bind2nd(std::mem_fun(&Sprite::setEnabled), visible));
  for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&Sprite::update));
}

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

