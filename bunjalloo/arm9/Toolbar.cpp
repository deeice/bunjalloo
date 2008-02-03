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
#include <algorithm>
#include <functional>
#include "libnds.h"
#include "Controller.h"
#include "Config.h"
#include "Document.h"
#include "URI.h"
#include "Image.h"
#include "Palette.h"
#include "Rectangle.h"
#include "Toolbar.h"
#include "View.h"
#include "Wifi9.h"
#include "Video.h"

using nds::Wifi9;

static const int TOOLBAR_Y(SCREEN_HEIGHT-17);
static const int TOOLBAR_Y_VERT(5);
static const int TOOLBAR_X(18);
static const int TOOLBAR_X_RIGHT(26);
static const int TOOLBAR_X_LEFT(10);
static const int TOOLBAR_SEP(20);
static const int TIMER_RESET(120);
static const int TOOLBAR_SCREEN(0);

// number of VRAM slots used per icon
static const int TILES_PER_ICON(8);

enum ToolbarIcon
{
  ICON_BACK = 0,
  ICON_FORWARD,
  ICON_STOP,
  ICON_REFRESH,

  ICON_BACK_DISABLE,
  ICON_FORWARD_DISABLE,
  ICON_PREFS,
  ICON_SPINNER,

  ICON_BOOKMARK,
  ICON_GO_URL,
  ICON_SEARCH,
  ICON_SAVE_AS,

  ICON_NOT_CONNECTED,
  ICON_CONNECTED,
  ICON_CONNECT_ERROR,
  ICON_HIDE_LEFT,

};

enum ToolbarSpriteID
{
  SPRITE_HIDE,

  SPRITE_BACK,
  SPRITE_FORWARD,
  SPRITE_STOP_REFRESH,
  SPRITE_BOOKMARK,

  SPRITE_GO_URL,
  SPRITE_SAVE_AS,

  SPRITE_CONNECT_STATUS,

  SPRITE_SPINNER,

  SPRITE_END_OF_ENTRIES,
  SPRITE_CURSOR=SPRITE_END_OF_ENTRIES,

};

Toolbar::Toolbar(Document & doc, Controller & cont, View & view):
  m_visible(false),
  m_hidden(false),
  m_document(doc),
  m_controller(cont),
  m_view(view),
  m_angle(0),
  m_position(BOTTOM)
{
  nds::Video & main(nds::Video::instance(0));
  main.blend(nds::Video::BLDMOD_OBJECT, 0, nds::Video::BLDMOD_BG3);
  main.setBlendAB(8,8);
  m_document.registerView(this);
  for (int i = 0; i < SPRITE_END_OF_ENTRIES; i++)
  {
    // would be 4 tiles per sprite (16x16 = 4*8x8)
    // but the tile count is for 4bpp tiles, 256 color 8bpp require twice as much vram.
    nds::Sprite * sprite(new nds::Sprite(TOOLBAR_SCREEN, 16, 16, i*TILES_PER_ICON, 256));
    m_sprites.push_back(sprite);
  }
  layout();

  Image image("/"DATADIR"/fonts/toolbar.png", true);
  if (image.isValid())
  {
    const unsigned short * data = image.data();
    if (data)
    {
      unsigned short * oamData = m_sprites.front()->oamData();

      // need to load in 16x16 tile order
      int bytesPerRow = image.width();
      int tiles16x = image.width() / 16;
      int tiles16y = image.height() / 16;
      int oamIndex = 0;
      for (int metay = 0; metay < tiles16y ; metay++)
      {
        for (int metax = 0; metax < tiles16x ; metax++)
        {
          // loop over the 8 by 8 tiles
          // startx = x + metax * 2;
          // starty = y + metay * 2;
          // tileIndex = starty * tilesX + startx;
          for (int y = 0; y < 2; y++)
          {
            for (int x = 0; x < 2; x++)
            {
              // copy tile at tileX, tileY to next oam slot
              int tileY = y + metay*2;
              int tileX = x + metax*2;
              // index into oam:
              unsigned short * oamSlot = &oamData[oamIndex*8*8/2];
              oamIndex++;
              const unsigned short * src = data + (tileY*8*bytesPerRow) + (tileX*8);
              for (int row = 0; row < 8; row++)
              {
                for (int col = 0; col < 4; col++, src+=2)
                {
                  unsigned short pixelPair = src[0]|(src[1]<<8);
                  *oamSlot++ = pixelPair;
                }
                src -= 8;
                src += bytesPerRow;
              }

            }
          }
        }
      }
      nds::ObjectPalette p(TOOLBAR_SCREEN);
      p.load(image.palette(), image.paletteSize()*2);
    }
  }
  setHidden(false);
  updateIcons();
}

void Toolbar::layout()
{
  for (int i = 0; i < SPRITE_END_OF_ENTRIES; i++)
  {
    int x, y;
    switch (m_position)
    {
      case LEFT:
        y = TOOLBAR_Y_VERT + (i*TOOLBAR_SEP);
        x = TOOLBAR_X_LEFT;
        break;

      case RIGHT:
        y = TOOLBAR_Y_VERT + (i*TOOLBAR_SEP);
        x = SCREEN_WIDTH - TOOLBAR_X_RIGHT;
        break;

      case TOP:
        x = TOOLBAR_X + (i*TOOLBAR_SEP);
        y = TOOLBAR_Y_VERT;
        break;

      default:
      case BOTTOM:
        x = TOOLBAR_X + (i*TOOLBAR_SEP);
        y = TOOLBAR_Y;
        break;
    }
    m_sprites[i]->setX(x);
    m_sprites[i]->setY(y);
  }
  setVisible();
}

static void deleteSprite(nds::Sprite * s)
{
  delete s;
}

Toolbar::~Toolbar()
{
  for_each(m_sprites.begin(), m_sprites.end(), deleteSprite);
  m_document.unregisterView(this);
}

void Toolbar::notify()
{
  updateIcons();
}

bool Toolbar::visible() const
{
  return m_visible;
}
void Toolbar::setVisible(bool visible)
{
  m_visible = visible;
  for_each(m_sprites.begin(), m_sprites.end(), std::bind2nd(std::mem_fun(&nds::Sprite::setEnabled), m_visible));
  m_sprites[SPRITE_SPINNER]->setEnabled( visible and (m_document.status() != Document::LOADED) );
  for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&nds::Sprite::update));
}

bool Toolbar::touch(int x, int y)
{
  nds::Rectangle touchZone = { m_sprites[0]->x(), m_sprites[0]->y(),
    m_sprites[SPRITE_END_OF_ENTRIES-1]->x() - m_sprites[0]->x() + m_sprites[0]->width(),
    m_sprites[SPRITE_END_OF_ENTRIES-1]->y() - m_sprites[0]->y() + m_sprites[0]->height()
    };
  if (not visible())
  {
    return false;
  }
  if (touchZone.hit(x, y))
  {
    bool handled(false);
    int index = 0;
    for (SpriteVector::iterator it(m_sprites.begin());
        it != m_sprites.end();
        ++it, ++index)
    {
      nds::Sprite * sprite(*it);
      nds::Rectangle rect = { sprite->x(), sprite->y(), sprite->width(), sprite->height() };
      if (rect.hit(x, y))
      {
        handlePress(index);
        handled = true;
      }
      // if hidden, only handle hide/show thing
      if (m_hidden and index)
      {
        break;
      }
    }
    return handled;
  }
  return false;
}

void Toolbar::handlePress(int i)
{
  switch ((ToolbarSpriteID)i)
  {
    case SPRITE_HIDE:
      setHidden(!m_hidden);
      break;
    case SPRITE_BACK:
      m_controller.previous();
      break;
    case SPRITE_FORWARD:
      m_controller.next();
      break;
    case SPRITE_STOP_REFRESH:
      if (m_document.status() == Document::LOADED)
      {
        m_controller.reload();
      }
      else
      {
        m_controller.stop();
      }
      break;
    case SPRITE_SAVE_AS:
      m_view.saveAs();
      break;
    case SPRITE_GO_URL:
      m_view.enterUrl();
      break;
    default:
      break;
  }
}

void Toolbar::updateIcons()
{
  m_sprites[SPRITE_HIDE]->setTile( TILES_PER_ICON * ICON_HIDE_LEFT);
  m_sprites[SPRITE_BACK]->setTile( TILES_PER_ICON * ( m_document.hasPreviousHistory() ? ICON_BACK: ICON_BACK_DISABLE));
  m_sprites[SPRITE_FORWARD]->setTile( TILES_PER_ICON * ( m_document.hasNextHistory() ? ICON_FORWARD: ICON_FORWARD_DISABLE));
  m_sprites[SPRITE_STOP_REFRESH]->setTile( TILES_PER_ICON * ( m_document.status() != Document::LOADED ? ICON_STOP: ICON_REFRESH));
  m_sprites[SPRITE_BOOKMARK]->setTile( TILES_PER_ICON * ICON_BOOKMARK);
  m_sprites[SPRITE_GO_URL]->setTile( TILES_PER_ICON * ICON_GO_URL);
  m_sprites[SPRITE_SAVE_AS]->setTile( TILES_PER_ICON * ICON_SAVE_AS);
  m_sprites[SPRITE_SPINNER]->setTile( TILES_PER_ICON * ICON_SPINNER);
  bool wifiInit = m_controller.wifiInitialised();
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
  m_sprites[SPRITE_CONNECT_STATUS]->setTile( TILES_PER_ICON * wifiIcon);
  m_sprites[SPRITE_SPINNER]->setEnabled( m_document.status() != Document::LOADED );

  m_sprites[SPRITE_HIDE]->setHflip(m_hidden);

}

void Toolbar::tick()
{
  if (visible())
  {
    if (m_document.status() != Document::LOADED)
    {
      m_angle+=32;
      m_angle &= 0x1ff;
      nds::Sprite * spinner(m_sprites[SPRITE_SPINNER]);
      spinner->setDoubleSize(false);
      spinner->setRotateScale(true);
      spinner->setRotate(1);
      u16 cosAng = COS[m_angle] / 16;
      u16 sinAng = SIN[m_angle] / 16;
      spinner->setAffine(cosAng, sinAng, -sinAng, cosAng);
    }
    for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&nds::Sprite::update));
  }
}

void Toolbar::cyclePosition()
{
  switch (m_position)
  {
    case TOP:
      m_position = RIGHT;
      break;
    case RIGHT:
      m_position = BOTTOM;
      break;
    case BOTTOM:
      m_position = LEFT;
      break;
    case LEFT:
      m_position = TOP;
      break;
    default:
      m_position = BOTTOM;
      break;
  }
  layout();
}

void Toolbar::showCursor(int x, int y, int cursorid)
{
  // plonk a cursor at the given position
  // coords in world space
  y -= 192;
  if (y < 0)
    return;
  m_cursorSprite->setEnabled();
  m_cursorSprite->update();
}

void Toolbar::hideCursor()
{
  m_cursorSprite->setEnabled(false);
  m_cursorSprite->update();
}

void Toolbar::setHidden(bool hidden)
{
  for_each(m_sprites.begin(), m_sprites.end(), std::bind2nd(std::mem_fun(&nds::Sprite::setEnabled), !hidden));
  m_sprites[SPRITE_SPINNER]->setEnabled( m_document.status() != Document::LOADED );
  for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&nds::Sprite::update));
  nds::Sprite * hideToggle(m_sprites[SPRITE_HIDE]);
  hideToggle->setEnabled();
  hideToggle->setHflip(hidden);
  if (hidden)
  {
    hideToggle->setX(0);
    hideToggle->setTranslucent(true);
  }
  else
  {
    hideToggle->setX(TOOLBAR_X);
    hideToggle->setTranslucent(false);
  }
  m_hidden = hidden;
}
