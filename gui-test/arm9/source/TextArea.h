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

    /** Set the cursor position. This is where the text will be "drawn" the
     * next time a print routine is called.
     * @param x the cursor x position in pixels.
     * @param y the cursor y position in pixels.
     */
    void setCursor(int x, int y);

    /** Append text to the text area.
     * @param unicodeString the text to append.
     */
    void appendText(const UnicodeString & unicodeString);

    /** Clear the text (empty the document model).
     */
    void clearText();

    /** Get the text that is held internally.
     */
    const UnicodeString & document() const;

    /** Get the current font.
     * @return the current font.
     */
    inline const Font & font() const;

    /** Set the font to use. TextArea takes control of the memory and deletes
     * on end/font reset.
     * @param font the font to use from now on.
     */
    void setFont(Font * font);

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

    /** Return the text colour to the default, initial value. */
    void setDefaultColor();

    /** Get the size of the text in pixels using the current font.
     * @param unicodeString the string to check the size of.
     * @return the size of the string in pixels.
     */
    int textSize(const UnicodeString & unicodeString) const;

    /** Get the editable status.
     * @return true if we can edit, false otherwise.
     */
    bool isEditable() const;

    /** Set the editable flag.
     * @param edit true if this is an editable text area.
     */
    void setEditable(bool edit=true);

    /** A hack for debugging.
     * @return the current document converted to a char string.
     */
    std::string asString() const;

    // Component method reimplementation.
    virtual void setSize(unsigned int w, unsigned int h);
    virtual void paint(const nds::Rectangle & clip);

  private:
    enum ControlState {
      TEXT,
      LINK,
      FORM
    };
    typedef std::list<Link*> LinkList;

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
    UnicodeString m_document;

    //LinkList m_links;

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
