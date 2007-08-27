/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef Font_h_seen
#define Font_h_seen

#include <string>
#include <vector>

class Range;

/** Encapsulates the Font handling. */
class Font
{
  public:

    //! A single character glyph.
    struct Glyph
    {
      //! Width of the glyph in pixels.
      int width;
      //! Height of the glyph in pixels.
      int height;
      //! Graphical data that can be used to draw the glyph.
      const unsigned char * data;
    };

    /** Construct a font from a file name. The filename should be the base name, 
     * .pal, .img and .map extensions are added to read in the appropriate sections.
     * @param fileName the base file name to use.
     */
    Font(const std::string & fileName);

    /** Construct a font from raw data.
     * @param imageData the font image data representing the way to draw glyphs.
     * @param mapData the data to map to glyph sizes and positions.
     */
    Font(const unsigned char * imageData, const unsigned char * mapData);

    //! Destructor.
    ~Font();

    /** Get the size of a given string of text.
     * @param text encoded text data.
     * @param textLength the number of bytes in the text.
     * @param width is filled with the width.
     * @param height is filled with the ehight.
     * @param encoding the type of encoding used - utf-8 or iso_8859-1.
     */
    void textSize(const char * text,
        int textLength, 
        int & width, 
        int & height, 
        const std::string & encoding="utf-8") const;
    void glyph(unsigned int glyphId, Glyph & g) const;

    /** Get the total width of all the font glyphs. This is the width of an
     * image that contains all available fonts.
     * @return the total width of the font glyphs.
     */
    inline int totalWidth() const;

    /** Get the font height. This is fixed.
     * @return font height.
     */
    inline int height() const;

  private:

    //! Total width of the image.
    unsigned int m_width;
    unsigned int m_height;
    const unsigned char * m_glyphData;
    unsigned char * m_glyphSize;
    unsigned short * m_glyphPosition;
    unsigned short * m_palette;
    std::vector<Range*> m_range;
    std::string m_encoding;

    int valueToIndex(unsigned int glyphId) const;
    int minGlyph() const;
    void init(const unsigned char * imageData, const unsigned char * mapData);
};

inline int Font::totalWidth() const
{
  return m_width;
}
inline int Font::height() const
{
  return m_height;
}
#endif
