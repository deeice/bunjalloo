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
#include "gif_lib.h"
#include "tinyjpeg.h"

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

static bool isGif(const char *filename)
{
   char buf[GIF_STAMP_LEN+1];

   nds::File f;
   f.open(filename, "rb");
   if (not f.is_open())
      return 0;

   /* Read in some of the signature bytes */
   if (f.read(buf, GIF_STAMP_LEN) != GIF_STAMP_LEN)
      return 0;

   buf[GIF_STAMP_LEN] = 0;
   /* Compare the first 3 bytes of the signature.  Return true if they match */
   return (strncmp(GIF_STAMP, buf, GIF_VERSION_POS) == 0);
}

class Array
{
  public:
    Array(int size)
      : m_array(new unsigned char[size]),
      m_size(size)
    {}

    ~Array()
    {
      delete [] m_array;
    }

    inline operator unsigned char* () const
    {
      return m_array;
    }

    inline operator char* () const
    {
      return (char*)m_array;
    }

    inline unsigned char & operator[] (int i)
    {
      return m_array[i];
    }

    unsigned int length() const
    {
      return m_size;
    }

  private:
    unsigned char * m_array;
    unsigned int m_size;
};

static bool isJpeg(const char * filename)
{
  // this is crap. Have to read in the whole file to see if it a JPEG.
  nds::File f;
  f.open(filename, "rb");
  if (f.is_open())
  {
    Array array(f.size());
    // what if it runs out of memory? new does what?
    f.read(array, array.length());
    f.close();
    struct jdec_private *jdec;
    jdec = tinyjpeg_init();
    if (jdec == 0)
    {
      return false;
    }
    if (tinyjpeg_parse_header(jdec, array, array.length())<0)
    {
      return false;
    }
    return true;
  }
  return false;
}

Image::ImageType Image::imageType(const char * filename)
{
  if (isPng(filename)) return Image::ImagePNG;
  if (isGif(filename)) return Image::ImageGIF;
  if (isJpeg(filename)) return Image::ImageJPEG;
  return Image::ImageUNKNOWN;
}

Image::Image(const char * filename, ImageType type, bool keepPalette):
  m_valid(false),
  m_keepPalette(keepPalette),
  m_width(0),
  m_height(0),
  m_channels(3),
  m_data(0),
  m_palette(0)
{
  ImageType actualType(imageType(filename));
  if (actualType != type)
  {
    // make your mind up
    return;
  }
  if (type == ImageUNKNOWN)
  {
    return;
  }
  readImage(filename, type);
}

Image::Image(const char * filename, bool keepPalette):
  m_valid(false),
  m_keepPalette(keepPalette),
  m_width(0),
  m_height(0),
  m_channels(3),
  m_data(0),
  m_palette(0)
{
  ImageType type(imageType(filename));
  readImage(filename, type);
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
   if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
     png_set_strip_alpha(png_ptr);
   
   // paletted images require this:
   if(info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) 
   {
     if (not m_keepPalette) {
       png_set_palette_to_rgb(png_ptr);
     }
     else
     {
       png_colorp png_palette;
       int palette_entries;
       png_get_PLTE(png_ptr,info_ptr, &png_palette, &palette_entries);
       m_paletteSize = palette_entries;
       m_palette = (unsigned short*)malloc(sizeof(unsigned short) * palette_entries);
       for (int i = 0; i < palette_entries; ++i)
       {
         m_palette[i] = RGB8(png_palette[i].red, png_palette[i].green, png_palette[i].blue);
       }
     }
   }

   png_set_interlace_handling(png_ptr);
   png_read_update_info(png_ptr, info_ptr);

   m_channels = png_get_channels(png_ptr, info_ptr);
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

static int user_read_gif(GifFileType * gif, GifByteType * data, int size)
{
  nds::File * f = (nds::File*)(gif->UserData);
  size_t n = f->read((char*)data, size);
  return n;
}

/** RAII wrapper around the GifFileType. */
class GifClass
{
  public:
    inline GifClass(const char * filename):m_buffer(0)
    {
      m_file.open(filename, "rb");
      m_gifFile = DGifOpen((void*)&m_file, user_read_gif);
    }

    inline ~GifClass()
    {
      if (m_gifFile)
        DGifCloseFile(m_gifFile);
      delete m_buffer;
    }

    // hackily throw in the screen buffer pointer too...
    inline void setScreenBuffer(unsigned char * buf)
    {
      m_buffer = buf;
    }

    inline operator GifFileType *()
    {
      return m_gifFile;
    }

    inline GifFileType * operator ->()
    {
      return m_gifFile;
    }

  private:
    GifFileType * m_gifFile;
    unsigned char * m_buffer;
    nds::File m_file;
};

static const int InterlacedOffset[] = { 0, 4, 2, 1 };
static const int InterlacedJumps[] = { 8, 8, 4, 2 };

void Image::readGif(const char * filename)
{
  m_valid = false;
  GifClass GifFile(filename);
  if (GifFile == 0)
  {
    return;
  }
  m_width = GifFile->SWidth;
  m_height = GifFile->SHeight;

  // this code is adapted from giflib's gif2rgb.c utility program.
  // ScreenBuffer holds the index into palette values.
  // FIXME: this is not very good yet - if the image is too big, what then?
  // also, it allocates 4 * the image width*height
  unsigned char * ScreenBuffer = (unsigned char*)malloc( m_height * m_width );
  if (ScreenBuffer == 0) {
    return;
  }
  GifFile.setScreenBuffer(ScreenBuffer);

  for (int i = 0; i < GifFile->SWidth*GifFile->SHeight; i++)
  {
    ScreenBuffer[i] = GifFile->SBackGroundColor;
  }

  GifRecordType RecordType;
  do {
    GifByteType *Extension;
    int Row, Col, Width, Height, ExtCode;
    if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
      return;
    }
    switch (RecordType) {
      case IMAGE_DESC_RECORD_TYPE:
        if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
          return;
        }
        Row = GifFile->Image.Top; /* Image Position relative to Screen. */
        Col = GifFile->Image.Left;
        Width = GifFile->Image.Width;
        Height = GifFile->Image.Height;

        if ((GifFile->Image.Left + GifFile->Image.Width) > GifFile->SWidth 
            or (GifFile->Image.Top + GifFile->Image.Height) > GifFile->SHeight)
        {
          return;
        }
        if (GifFile->Image.Interlace) {
          /* Need to perform 4 passes on the images: */
          for (int i = 0; i < 4; i++)
            for (int j = Row + InterlacedOffset[i]; j < Row + Height;
                j += InterlacedJumps[i])
            {
              if (DGifGetLine(GifFile, &ScreenBuffer[j*m_width +  Col],
                    Width) == GIF_ERROR)
              {
                return;
              }
            }
        }
        else {
          for (int i = 0; i < Height; i++, Row++) {
            if (DGifGetLine(GifFile, &ScreenBuffer[ Row*m_width+Col], Width) == GIF_ERROR) 
            {
              return;
            }
          }
        }
        break;
      case EXTENSION_RECORD_TYPE:
        /* Skip any extension blocks in file: */
        if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
          return;
        }
        while (Extension != NULL) {
          if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
            return;
          }
        }
        break;
      case TERMINATE_RECORD_TYPE:
        break;
      default:		    /* Should be traps by DGifGetRecordType. */
        break;
    }
  }
  while (RecordType != TERMINATE_RECORD_TYPE);

  // map ScreenBuffer to m_data
  ColorMapObject * ColorMap = (GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
  m_data = (unsigned char * ) malloc(3*m_width*m_height);
  unsigned char * dest = m_data;
  for (unsigned int i = 0; i < m_height; i++)
  {
    unsigned char * GifRow = &ScreenBuffer[i*m_width];
    for (unsigned int j = 0; j < m_width; j++)
    {
      GifColorType * ColorMapEntry = &ColorMap->Colors[GifRow[j]];
      *dest++ = ColorMapEntry->Red;
      *dest++ = ColorMapEntry->Green;
      *dest++ = ColorMapEntry->Blue;
    }
  }
  m_valid = true;
}

void Image::readJpeg(const char * filename)
{
  m_valid = false;
  nds::File f;
  f.open(filename, "rb");
  if (not f.is_open())
  {
    return;
  }
  Array array(f.size());
  // what if it runs out of memory? new does what?
  f.read(array, array.length());
  f.close();
  struct jdec_private *jdec;
  jdec = tinyjpeg_init();
  if (jdec == 0)
  {
    return;
  }
  if (tinyjpeg_parse_header(jdec, array, array.length())<0)
  {
    return;
  }
  tinyjpeg_get_size(jdec, &m_width, &m_height);
  if (tinyjpeg_decode(jdec, TINYJPEG_FMT_RGB24) < 0)
  {
    return;
  }
  unsigned char * components[3];
  tinyjpeg_get_components(jdec, components);
  m_data = components[0];
  m_channels = 3;
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

void Image::readImage(const char *filename, ImageType type)
{
  switch (type)
  {
    case ImagePNG:
      readPng(filename);
      break;
    case ImageGIF:
      readGif(filename);
      break;
    case ImageJPEG:
      readJpeg(filename);
      break;
    case ImageUNKNOWN:
      break;
  }
}

unsigned int Image::channels() const
{
  return m_channels;
}
