#include "libnds.h"
#include "ImageComponent.h"
#include "Image.h"
#include "URI.h"
#include "Canvas.h"
#include "Palette.h"
using nds::Canvas;
ImageComponent::ImageComponent(const std::string & filename):m_image(0)
{
  if (not filename.empty())
  {
    printf("Image '%s'\n", filename.c_str());
    m_image = new Image(filename.c_str());
    printf("Here %d %d\n", m_image->width(), m_image->height());
    setSize(m_image->width(), m_image->height());
  }
}

ImageComponent::~ImageComponent()
{
  delete m_image;
}

static void drawImage(Canvas & canvas, Image & image, int startx, int starty)
{
  printf("Image %d %d %d %d\n", startx, starty, image.width(), image.height());
  const unsigned char * data = image.data();
  canvas.fillRectangle(startx, starty, image.width(), image.height(), nds::Color(21,21,21));
  for (unsigned int y = 0; y < image.height(); ++y)
  {
    const unsigned char * row = &data[y*image.width()*3];
    for (unsigned int x = 0; x < image.width(); ++x, row+=3)
    {
      canvas.drawPixel(startx+x, starty+y, RGB8(row[0], row[1], row[2]));
    }
  }
}

void ImageComponent::paint(const nds::Rectangle & clip)
{
  if (m_image)
  {
    printf("Render png\n");
    //nds::Canvas::instance().drawRectangle(x(),y(),width(),height(), nds::Color(31,0,0));
    drawImage(Canvas::instance(), *m_image, x(), y());
  }
  else
  {
    Canvas::instance().drawRectangle(0,0,128,128, nds::Color(31,0,0));
  }
}
