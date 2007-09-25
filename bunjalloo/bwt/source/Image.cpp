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
#include "Image.h"
#include "File.h"
#include "png.h"

static const int PNG_BYTES_TO_CHECK = 8;

static bool isPng(const char *filename)
{
   png_byte buf[PNG_BYTES_TO_CHECK];

   nds::File f;
   f.open(filename, "rb");
   /* Open the prospective PNG file. */
   if (not f.is_open())
      return 0;

   /* Read in some of the signature bytes */
   if (f.read((char*)buf, PNG_BYTES_TO_CHECK) != PNG_BYTES_TO_CHECK)
      return 0;

   /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
      Return nonzero (true) if they match */
   return(!png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}

Image::Image(const char * filename):
  m_valid(false),
  m_width(0),
  m_height(0),
  m_data(0),
  m_palette(0)
{
  if (not isPng(filename))
  {
    return;
  }
  readPng(filename);

}

Image::~Image()
{
  free(m_data);
  free(m_palette);
}

bool Image::isValid() const
{
  return m_valid;
}

unsigned int Image::width() const
{
  return m_width;
}

unsigned int Image::height() const
{
  return m_height;
}

// RGB data.
const unsigned char * Image::data() const
{
  return m_data;
}

static void user_read_fn(png_structp png_ptr, unsigned char *data, png_size_t size)
{
  nds::File * f = (nds::File*)png_get_io_ptr(png_ptr);
  size_t n = f->read((char*)data, size);
  if(size && (n == 0)) {
    // meh
  }
}

void Image::readPng(const char * filename)
{
   m_valid = false;
   nds::File f;
   f.open(filename, "rb");
   if (not f.is_open())
   {
     return;
   }

   png_structp png_ptr= png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL)
   {
      return;
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
      return;
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
      /* If we get here, we had a problem reading the file */
      return;
   }

   // compiled png with NO_STIO - avoids sprintf madness.
   png_set_read_fn(png_ptr, (void *)&f, user_read_fn);
   // read the easy way
   png_read_info(png_ptr, info_ptr);
   // alpha images require this:
   png_set_strip_alpha(png_ptr);
   
   // paletted images require this:
   if(info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) 
   {
#if 0
     png_set_palette_to_rgb(png_ptr);
#else
     // png_set_palette_to_rgb(png_ptr);
     png_colorp png_palette;
     int palette_entries;
     png_get_PLTE(png_ptr,info_ptr, &png_palette, &palette_entries);
     m_paletteSize = palette_entries;
     m_palette = (unsigned short*)malloc(sizeof(unsigned short) * palette_entries);
     for (int i = 0; i < palette_entries; ++i)
     {
       m_palette[i] = RGB8(png_palette[i].red, png_palette[i].green, png_palette[i].blue);
     }
#endif
   }
   png_set_interlace_handling(png_ptr);
   png_read_update_info(png_ptr, info_ptr);

   m_height = info_ptr->height;
   m_width = info_ptr->width;
   m_data = (unsigned char*)malloc( info_ptr->rowbytes * m_height);
   png_bytep * row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * m_height);
   if (row_pointers == 0)
   {
     return;
   }
   for (unsigned int y = 0; y < m_height; y++)
   {
     row_pointers[y] = (png_bytep)(m_data + (info_ptr->rowbytes * y));
   }

   png_read_image(png_ptr, row_pointers);

   /* clean up after the read, and free any memory allocated - REQUIRED */
   // clear up row pointers - now everything is in m_data
   free(row_pointers);
   png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

   /* that's it */
   m_valid = true;
}
unsigned int Image::paletteSize() const
{
  return m_paletteSize;
}
const unsigned short * Image::palette() const
{
  return m_palette;
}
