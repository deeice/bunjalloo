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
const int Toolbar::TOOLBAR_X(18);
const int Toolbar::TOOLBAR_X_RIGHT(26);
const int Toolbar::TOOLBAR_X_LEFT(10);
const int Toolbar::TOOLBAR_SEP(20);
static const int TIMER_RESET(120);
static const int TOOLBAR_SCREEN(0);

// number of VRAM slots used per icon
const int Toolbar::TILES_PER_ICON(8);

//VRAM initialisation - done just the once.
static void initSpriteData(unsigned short * oamData)
{
  static bool s_haveInitialised(false);
  if (s_haveInitialised)
    return;

  nds::Video & main(nds::Video::instance(0));
  main.blend(nds::Video::BLDMOD_OBJECT, 0, nds::Video::BLDMOD_BG3);
  main.setBlendAB(8,8);

  Image image("/"DATADIR"/fonts/toolbar.png", true);
  if (image.isValid())
  {
    const unsigned short * data = image.data();
    if (data)
    {
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
  s_haveInitialised = true;
}

Toolbar::Toolbar(Document & doc, Controller & cont, View & view, int entries):
  m_document(doc),
  m_controller(cont),
  m_view(view),
  m_visible(false),
  m_position(BOTTOM)
{
  m_document.registerView(this);
  for (int i = 0; i < entries; i++)
  {
    // would be 4 tiles per sprite (16x16 = 4*8x8)
    // but the tile count is for 4bpp tiles, 256 color 8bpp require twice as much vram.
    nds::Sprite * sprite(new nds::Sprite(TOOLBAR_SCREEN, 16, 16, i*TILES_PER_ICON, 256));
    m_sprites.push_back(sprite);
  }
  initSpriteData(m_sprites.front()->oamData());
  layout();
}

void Toolbar::layout()
{
  for (unsigned int i = 0; i < m_sprites.size(); i++)
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
  setVisible(visible());
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
  for_each(m_sprites.begin(), m_sprites.end(), std::bind2nd(std::mem_fun(&nds::Sprite::setEnabled), visible));
  for_each(m_sprites.begin(), m_sprites.end(), std::mem_fun(&nds::Sprite::update));
}

bool Toolbar::touch(int x, int y)
{
  nds::Sprite * first(m_sprites.front());
  nds::Sprite * last(m_sprites.back());
  nds::Rectangle touchZone = { first->x(), first->y(),
    last->x() - first->x() + first->width(),
    last->y() - first->y() + first->height()
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
      if (sprite->enabled() and rect.hit(x, y))
      {
        handlePress(index);
        handled = true;
      }
    }
    return handled;
  }
  return false;
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

Toolbar::Position Toolbar::position() const
{
  return m_position;
}
