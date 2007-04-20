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
    Font(unsigned char * imageData, unsigned char * mapData);

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
    unsigned char * m_glyphData;
    unsigned char * m_glyphSize;
    unsigned short * m_glyphPosition;
    unsigned short * m_palette;
    std::vector<Range*> m_range;
    std::string m_encoding;

    int valueToIndex(unsigned int glyphId) const;
    int minGlyph() const;
    void init(unsigned char * imageData, unsigned char * mapData);
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
