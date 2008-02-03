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
#ifndef Toolbar_h_seen
#define Toolbar_h_seen

#include <vector>
#include "Sprite.h"

class Document;
class Controller;
class View;

class Toolbar: public ViewI
{
  public:
    enum Position
    {
      TOP,
      BOTTOM,
      LEFT,
      RIGHT
    };

    Toolbar(Document & doc, Controller & cont, View & view);
    ~Toolbar();

    bool visible() const;
    void setVisible(bool visible=true);

    void tick();

    bool touch(int x, int y);

    void updateIcons();

    void cyclePosition();

    virtual void notify();

    void showCursor(int x, int y, int cursorid);
    void hideCursor();

  private:
    // visible - ie. is showing
    bool m_visible;
    // hidden - ie. not minimized
    bool m_hidden;
    typedef std::vector<nds::Sprite * > SpriteVector;
    SpriteVector m_sprites;
    nds::Sprite *  m_cursorSprite;
    Document & m_document;
    Controller & m_controller;
    View & m_view;

    int m_angle;
    Position m_position;

    void handlePress(int i);
    void layout();

    void setHidden(bool hidden);
};
#endif
