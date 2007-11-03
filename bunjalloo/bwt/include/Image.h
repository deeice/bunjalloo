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
#ifndef Image_h_seen
#define Image_h_seen

class Image
{
  public:
    enum ImageType
    {
      ImagePNG,
      ImageGIF,
      ImageJPEG,
      ImageUNKNOWN
    };

    /** Create an image from the given file name, using the given ImageType.
     * @param filename the name of the file to open.
     * @param type the type of the image - usually given by Mime Type, etc.
     * @param keepPalette if the image is recognised as having palette data,
     *   then if this flag is true the palette will be kept. Otherwise the
     *   palette data will be lost (converted to RGB triplets vs indexed)
     */
    Image(const char * filename, ImageType type, bool keepPalette=false);

    /** Create an image from the given file name.
     * @param filename the name of the file to open.
     * @param keepPalette if the image is recognised as having palette data,
     *   then if this flag is true the palette will be kept. Otherwise the
     *   palette data will be lost (converted to RGB triplets vs indexed)
     */
    Image(const char * filename, bool keepPalette=false);

    /** Free up the data.*/
    ~Image();

    /** Is this valid or not.
     * @return true if the image is valid, false otherwise.
     */
    bool isValid() const;

    /** Get the width.
     * @return the image width in pixels.
     */
    unsigned int width() const;

    /** Get the height.
     * @return the image height in pixels.
     */
    unsigned int height() const;

    /** Get the size of the palette, if this is a palettized image and the
     * keepPalette flag was used.
     * @return the number of colours in the palette.
     */
    unsigned int paletteSize() const;

    unsigned int channels() const;

    /** Get a pointer to the palette data.
     * @return the pointer to the palette data.
     * @see paletteSize()
     */
    const unsigned short * palette() const;


    /** Get the RGB or indexed image data.
     * @return pointer to the pixel data.
     */
    const unsigned short * data() const;

  private:

    bool m_valid;
    bool m_keepPalette;

    // scaled w, h
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_currentLine;

    // actual w, h of image
    unsigned int m_realWidth;
    unsigned int m_realHeight;
    // bits per pixel when decoding the line
    unsigned int m_bpp;

    unsigned int m_paletteSize;
    unsigned int m_channels;
    unsigned short * m_data;
    unsigned short * m_palette;

    void readPng(const char *filename);
    void readGif(const char *filename);
    void readJpeg(const char *filename);
    void readImage(const char *filename, ImageType type);
    static ImageType imageType(const char * filename);

    /** Render line n from the given data. */
    void renderLine(const unsigned char * line, int n);
    void calculateScale();

};
#endif
