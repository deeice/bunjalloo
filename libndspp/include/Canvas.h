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
#ifndef Canvas_h_seen
#define Canvas_h_seen

#include "Rectangle.h"

namespace nds   // tolua_export
{               // tolua_export
  class Background;

// tolua_begin
  class Canvas
  {
    public:
      static Canvas & instance();
      void drawPixel(int x, int y, int color);
      void fillRectangle(int x, int y, int w, int h, int color);
      void drawRectangle(int x, int y, int w, int h, int color);
      void verticalLine(int x, int y, int length, int colour);
      void horizontalLine(int x, int y, int length, int colour);
      int width() const;
      int height() const;
      void endPaint();

      void setClip(const nds::Rectangle & clip);
      nds::Rectangle clip() const;
// tolua_end

    private:
      Background * m_bgMain;
      Background * m_bgSub;
      unsigned short * m_frontMain;
      unsigned short * m_backMain;
      unsigned short * m_frontSub;
      unsigned short * m_backSub;
      Rectangle m_clip;

      Canvas();
      ~Canvas();

      unsigned short * vram();

      Canvas(const Canvas &);
      const Canvas operator=(const Canvas &);
      unsigned short * vram(int y);


  }; //tolua_export
} //tolua_export
#endif
