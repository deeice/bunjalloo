#include <string>
#include <algorithm>
#include <functional>
#include "libnds.h"
#include "BrowseToolbar.h"
#include "Controller.h"
#include "Document.h"
#include "View.h"
#include "Wifi9.h"

using namespace nds;

enum ToolbarSpriteID
{
  SPRITE_HIDE,

  SPRITE_BACK,
  SPRITE_FORWARD,
  SPRITE_STOP_REFRESH,
  SPRITE_BOOKMARK,

  SPRITE_GO_URL,
  SPRITE_PREFS,
  SPRITE_SAVE_AS,

  SPRITE_CONNECT_STATUS,

  SPRITE_END_OF_ENTRIES,

};

BrowseToolbar::BrowseToolbar(View & view):
  Toolbar(view, SPRITE_END_OF_ENTRIES),
  m_angle(0),
  m_hidden(false)
{
  updateIcons();
  setHidden(false);
}

void BrowseToolbar::tick()
{
  if (visible())
  {
    Sprite * spinner(m_sprites[SPRITE_CONNECT_STATUS]);
    if (m_view.document().status() != Document::LOADED)
    {
      spinner->setTile(TILES_PER_ICON * ICON_SPINNER);
      m_angle+=32;
      m_angle &= 0x1ff;
      spinner->setDoubleSize(false);
      spinner->setRotateScale(true);
      spinner->setRotate(1);
      u16 cosAng = COS[m_angle] / 16;
      u16 sinAng = SIN[m_angle] / 16;
      spinner->setAffine(cosAng, sinAng, -sinAng, cosAng);
    }
    else
    {
      spinner->setRotateScale(false);
    }
    for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&Sprite::update));
  }
}

void BrowseToolbar::setVisible(bool visible)
{
  Toolbar::setVisible(visible);
  if (visible)
    setHidden(m_hidden);
}

void BrowseToolbar::handlePress(int i)
{
  switch ((ToolbarSpriteID)i)
  {
    case SPRITE_HIDE:
      setHidden(!m_hidden);
      break;
    case SPRITE_BACK:
      m_view.controller().previous();
      break;
    case SPRITE_FORWARD:
      m_view.controller().next();
      break;
    case SPRITE_STOP_REFRESH:
      if (m_view.document().status() == Document::LOADED)
      {
        m_view.controller().reload();
      }
      else
      {
        m_view.controller().stop();
      }
      break;
    case SPRITE_SAVE_AS:
      m_view.saveAs();
      break;
    case SPRITE_GO_URL:
      m_view.enterUrl();
      break;

    case SPRITE_PREFS:
      // Preferences...
      m_view.preferences();
      break;

    case SPRITE_BOOKMARK:
      m_view.bookmarkUrl();
      break;

    default:
      break;
  }
}

void BrowseToolbar::updateIcons()
{
  m_sprites[SPRITE_HIDE]->setTile( TILES_PER_ICON * ICON_HIDE_LEFT);
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ( m_view.document().hasPreviousHistory() ? ICON_BACK: ICON_BACK_DISABLE));
  m_sprites[SPRITE_FORWARD]->setTile( TILES_PER_ICON * ( m_view.document().hasNextHistory() ? ICON_FORWARD: ICON_FORWARD_DISABLE));
  m_sprites[SPRITE_STOP_REFRESH]->setTile( TILES_PER_ICON * ( m_view.document().status() != Document::LOADED ? ICON_STOP: ICON_REFRESH));
  m_sprites[SPRITE_BOOKMARK]->setTile( TILES_PER_ICON * ICON_BOOKMARK);
  m_sprites[SPRITE_GO_URL]->setTile( TILES_PER_ICON * ICON_GO_URL);
  m_sprites[SPRITE_SAVE_AS]->setTile( TILES_PER_ICON * ICON_SAVE_AS);
  m_sprites[SPRITE_PREFS]->setTile( TILES_PER_ICON * ICON_PREFS);
  bool wifiInit = m_view.controller().wifiInitialised();
  ToolbarIcon wifiIcon(ICON_NOT_CONNECTED);
  if (wifiInit)
  {
    switch ( Wifi9::instance().status() )
    {
      case Wifi9::CANNOTCONNECT:
        wifiIcon = ICON_CONNECT_ERROR;
        break;

      case Wifi9::SEARCHING:
      case Wifi9::AUTHENTICATING:
      case Wifi9::ASSOCIATING:
      case Wifi9::ACQUIRINGDHCP:
      case Wifi9::ASSOCIATED:
        wifiIcon = ICON_CONNECTED;
        break;

      case Wifi9::DISCONNECTED :
        wifiIcon = ICON_NOT_CONNECTED;
        break;
    }
  }
  if (m_view.document().status() == Document::LOADED)
  {
    m_sprites[SPRITE_CONNECT_STATUS]->setTile( TILES_PER_ICON * wifiIcon);
  }
  else
  {
    m_sprites[SPRITE_CONNECT_STATUS]->setTile( TILES_PER_ICON * ICON_SPINNER);
  }
  m_sprites[SPRITE_HIDE]->setHflip(m_hidden);
}

void BrowseToolbar::setHiddenIconContract()
{
  Sprite * hideToggle(m_sprites[SPRITE_HIDE]);
  switch (position())
  {
    case LEFT:
    case RIGHT:
    case TOP:
    case BOTTOM:
      hideToggle->setX(0);
      break;
  }
  hideToggle->setTranslucent(true);
}

void BrowseToolbar::setHiddenIconExpand()
{
  Sprite * hideToggle(m_sprites[SPRITE_HIDE]);
  switch (position())
  {
    case LEFT:
      hideToggle->setX(TOOLBAR_X_LEFT);
      break;
    case RIGHT:
      hideToggle->setX(SCREEN_WIDTH - TOOLBAR_X_RIGHT);
      break;
    case TOP:
    case BOTTOM:
      hideToggle->setX(TOOLBAR_X);
      break;
  }
  hideToggle->setTranslucent(false);
}

void BrowseToolbar::setHidden(bool hidden)
{
  for_each(m_sprites.begin(), m_sprites.end(), std::bind2nd(std::mem_fun(&Sprite::setEnabled), !hidden));
  for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&Sprite::update));
  Sprite * hideToggle(m_sprites[SPRITE_HIDE]);
  hideToggle->setEnabled();
  hideToggle->setHflip(hidden);
  if (hidden)
  {
    setHiddenIconContract();
  }
  else
  {
    setHiddenIconExpand();
  }
  m_hidden = hidden;
}

void BrowseToolbar::layout()
{
  Toolbar::layout();
  setHidden(m_hidden);
}
