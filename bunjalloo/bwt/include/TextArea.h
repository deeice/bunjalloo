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
    /** Sets the initial state of the TextArea.
     * @param font the font to use. This is not deleted.
     */
    TextArea(Font * font);

    /** Destructor. Deletes the palette data. */
    ~TextArea();

    /** Set the palette.
     * @param fileName the name of the font palette.
     */
    void setPalette(const std::string & fileName);

    /** Set the palette.
     * @param data the palette data
     * @param size the size of the palette data
     */
    void setPalette(const char * data, unsigned int size);

    /** Append text to the text area.
     * @param unicodeString the text to append.
     */
    void appendText(const UnicodeString & unicodeString);

    /** Clear the text (empty the document model).
     */
    void clearText();

    /** Get the text that is held internally.
     * @param returnString the return value.
     */
    void document(UnicodeString & returnString) const;

    /** Get the current font.
     * @return the current font.
     */
    inline const Font & font() const;

    /** Set the font to use. TextArea takes control of the memory and deletes
     * on end/font reset.
     * @param font the font to use from now on.
     */
    void setFont(Font * font);

    /** Set the text colour. Since the text is anti aliased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setTextColor(unsigned short color);

    /** Set the background colour. Since the text is anti aliased, this creates a coloured
     * scale palette based on the original grey-scale palette.
     * @param color the new text colour in 15 bit RGB format.
     */
    void setBackgroundColor(unsigned short color);

    /** Return the text colour to the default, initial value. */
    void setDefaultColor();

    /** Get the size of the text in pixels using the current font.
     * @param unicodeString the string to check the size of.
     * @return the size of the string in pixels.
     */
    int textSize(const UnicodeString & unicodeString) const;

    /** A hack for debugging.
     * @return the current document converted to a char string.
     */
    std::string asString() const;

    inline int cursorY() const;
    inline int cursorX() const;
    /** Get the number of lines that will be skipped when painting.*/
    int linesToSkip() const;

    inline bool parseNewline() const;
    inline void setParseNewline(bool parse=true);
    inline void setUnderline(bool underline=true);
    inline bool underline() const;

    // Component method reimplementation.
    virtual void setSize(unsigned int w, unsigned int h);
    virtual void paint(const nds::Rectangle & clip);

  protected:
    /** The document model contains one UnicodeString per line of text.*/
    std::vector<UnicodeString> m_document;

    /** Perform layout of the text. This is a pretty costly procedure in terms
     * of memory and CPU.
     */
    void layoutText();

    /** Get the current (last) line of text.
     * @return the last line of text in the document vector.
     */
    inline UnicodeString & currentLine();

    /** Called for each line of text that is visible. */
    virtual void printu(const UnicodeString & unicodeString);
    /** Print a single character. */
    bool doSingleChar(unsigned int value);


  private:

    Font * m_font;
    unsigned short * m_palette;
    unsigned short * m_basePalette;
    int m_paletteLength;
    bool m_parseNewline;
    int m_cursorx;
    int m_cursory;
    int m_initialCursorx;
    unsigned short m_bgCol;
    unsigned short m_fgCol;
    //! Position that the current line is at.
    int m_appendPosition;
    bool m_underLine;


    void printAt(Font::Glyph & g, int xPosition, int yPosition);
    void incrLine();
    inline void checkLetter(Font::Glyph & g);
    

    const UnicodeString nextWord(const UnicodeString & unicodeString, 
        int currPosition) const;
    void advanceWord(const UnicodeString & unicodeString, int wordLength,
        int & currPosition, UnicodeString::const_iterator & it) const;

    /** Set the cursor position. This is where the text will be "drawn" the
     * next time a print routine is called.
     * @param x the cursor x position in pixels.
     * @param y the cursor y position in pixels.
     */
    void setCursor(int x, int y);
};

// inline implementations
const Font & TextArea::font() const {
  return *m_font;
}

UnicodeString & TextArea::currentLine()
{
  if (m_document.empty())
    m_document.push_back(UnicodeString());
  return m_document.back();
}
int TextArea::cursorY() const
{
  return m_cursory;
}
int TextArea::cursorX() const
{
  return m_cursory;
}
void TextArea::setUnderline(bool underline)
{
  m_underLine = underline;
}
bool TextArea::underline() const
{
  return m_underLine;
}
bool TextArea::parseNewline() const
{
    return m_parseNewline;
}
void TextArea::setParseNewline(bool parse)
{
    m_parseNewline = parse;
}
#endif
