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
#ifndef TextArea_h_seen
#define TextArea_h_seen

#include "Font.h"
#include "UnicodeString.h"
#include "Component.h"
#include <list>

class Link;
class FormControl;
/** A widget for displaying text.*/
class TextArea : public Component
{
  public:
    /** Constructor.*/
    TextArea(Font * font);
    /** Destructor.*/
    ~TextArea();

    /** Set the palette.
     * @param fileName the name of the font palette.
     */
    void setPalette(const std::string & fileName);

    virtual void paint(const nds::Rectangle & clip);

    /** Set the palette.
     * @param data the palette data
     * @param size the size of the palette data
     */
    void setPalette(const char * data, unsigned int size);

    /** Set the cursor position. This is where the text will be "drawn" the
     * next time a print routine is called.
     * @param x the cursor x position in pixels.
     * @param y the cursor y position in pixels.
     */
    void setCursor(int x, int y);

    /** Print encoded text using the current encoding.
     * @param data the encoded text data.
     * @param amount the number of bytes of encoded data.
     */
    void print(const char * data, int amount);

    /** Append text to the text area.
     * @param unicodeString the text to append.
     */
    void appendText(const UnicodeString & unicodeString);

    /** Clear the text.
     */
    void clearText();

    /** Print a unicode string starting at the current cursor position. The
     * values are not decoded from the string.
     * @param unicodeString a string containing unicode values.
     */
    //void printu(const UnicodeString & unicodeString);

    /** Get the current font.
     * @return the current font.
     */
    inline const Font & font() const;

    /** Set the font to use. TextArea takes control of the memory and deletes
     * on end/font reset.
     * @param font the font to use from now on.
     */
    void setFont(Font * font);

    /** Set the current encoding. @see print().
     * @param encoding the encoding to use for all encoded text.
     */
    void setEncoding(const std::string & encoding="utf-8");

    /** Set the text colour. Since the text is antialiased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setTextColor(unsigned short color);

    /** Set the background colour. Since the text is antialiased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setBackgroundColor(unsigned short color);

    void setDefaultColor();
    void clear();

    virtual void setSize(unsigned int w, unsigned int h);

    int textSize(const UnicodeString & unicodeString) const;

    std::string asString() const;
  private:
    Font * m_font;
    unsigned short * m_palette;
    unsigned short * m_basePalette;
    int m_paletteLength;
    std::string m_encoding;
    int m_startLine;
    bool m_foundPosition;
    bool m_parseNewline;
    enum ControlState {
      TEXT,
      LINK,
      FORM
    };
    ControlState m_currentControl;
    UnicodeString m_document;
    UnicodeString m_textLine;


    int m_cursorx;
    int m_cursory;
    int m_initialCursorx;
    int m_initialCursory;
    typedef std::list<Link*> LinkList;
    LinkList m_links;

    unsigned short m_bgCol;
    unsigned short m_fgCol;
    int m_indentLevel;

    //! Position that the current line cursor is at.
    int m_appendPosition;

    void printAt(Font::Glyph & g, int xPosition, int yPosition);
    void incrLine();
    void checkLetter(Font::Glyph & g);
    bool doSingleChar(unsigned int value);
    
    void removeClickables();

    const UnicodeString nextWord(const UnicodeString & unicodeString, 
        int currPosition) const;
    void advanceWord(const UnicodeString & unicodeString, int wordLength,
        int & currPosition, UnicodeString::const_iterator & it) const;
    void printu(const UnicodeString & unicodeString);
};

// inline implementations
const Font & TextArea::font() const {
  return *m_font;
}
#endif
