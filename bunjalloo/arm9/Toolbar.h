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
    Toolbar(Document & doc, Controller & cont, View & view);
    ~Toolbar();

    bool visible() const;
    void setVisible(bool visible=true);

    void tick();

    bool touch(int x, int y);

    void updateIcons();

    virtual void notify();

  private:
    bool m_visible;
    typedef std::vector<nds::Sprite * > SpriteVector;
    SpriteVector m_sprites;
    Document & m_document;
    Controller & m_controller;
    View & m_view;

    int m_timerReset;
    int m_timer;
    int m_angle;

    void handlePress(int i);

};
#endif
