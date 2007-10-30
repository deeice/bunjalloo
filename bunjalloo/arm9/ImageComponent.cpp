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
#include "libnds.h"
#include "ImageComponent.h"
#include "Image.h"
#include "URI.h"
#include "Canvas.h"
#include "Palette.h"

using nds::Canvas;


ImageComponent::ImageComponent(Image * image):m_image(image)
{
  if (m_image)
  {
    setSize(m_image->width(), m_image->height());
  }
}

ImageComponent::~ImageComponent()
{
  delete m_image;
}

static void drawImage(Canvas & canvas, Image & image, int startx, int starty)
{
  const unsigned short * data = image.data();
  for (unsigned int y = 0; y < image.height(); ++y)
  {
    const unsigned short * row = &data[y*image.width()];
    for (unsigned int x = 0; x < image.width(); ++x, row++)
    {
      canvas.drawPixel(startx+x, starty+y, *row);
    }
  }
}

void ImageComponent::paint(const nds::Rectangle & clip)
{
  if (m_image)
  {
    //nds::Canvas::instance().drawRectangle(x(),y(),width(),height(), nds::Color(31,0,0));
    drawImage(Canvas::instance(), *m_image, x(), y());
  }
  else
  {
    Canvas::instance().drawRectangle(0,0,128,128, nds::Color(31,0,0));
  }
}
