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
